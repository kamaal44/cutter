#include "common/Configuration.h"
#include "common/Configuration.h"
#include "ColorSchemePrefWidget.h"
#include "ui_ColorSchemePrefWidget.h"
#include "common/ColorSchemeFileSaver.h"

#include <QMap>
#include <QFile>
#include <QDebug>
#include <QPainter>
#include <QJsonArray>
#include <QMouseEvent>
#include <QApplication>
#include <QColorDialog>
#include <QJsonDocument>

struct OptionIfo {
    QString info;
    QString displayingtext;
    bool isUsedStandardTextColor;
};

static const QMap<QString, OptionIfo> optionInfoMap = {
    {
        "comment", {
            QObject::tr("Color of comment generated by radare2"),
            QObject::tr("Comment"), false
        }
    },
    {
        "usrcmt", {
            QObject::tr("Comment created by user"),
            QObject::tr("Color of user Comment"), false
        }
    },
    {
        "args", {
            QObject::tr("Color of function arguments"),
            QObject::tr("Arguments"), false
        }
    },
    {
        "fname", {
            QObject::tr("Color of names of functions"),
            QObject::tr("Function name"), false
        }
    },
    {
        "floc", {
            QObject::tr("Color of function location"),
            QObject::tr("Function location"), false
        }
    },
    {
        "fline", {
            "",
            "fline", false
        }
    },
    {
        "flag", {
            QObject::tr("Color of flags (similar to bookmarks for offset)"),
            QObject::tr("Flag"), false
        }
    },
    { "label", { "", QObject::tr("Label"), false } },
    { "help", { "", QObject::tr("Help"), false } },
    { "flow", { "", QObject::tr("flow"), false } },
    { "flow2", { "", QObject::tr("flow2"), false } },
    { "prompt", { QObject::tr("Info"), QObject::tr("prompt"), false } },
    {
        "offset", {
            QObject::tr("Color of offsets"),
            QObject::tr("Offset"), false
        }
    },
    { "input", { QObject::tr("Info"), QObject::tr("input"), false } },
    {
        "invalid", {
            QObject::tr("Invalid opcode color"),
            QObject::tr("invalid"), false
        }
    },
    { "other", { "", QObject::tr("other"), false } },
    {
        "b0x00", {
            QObject::tr("0x00 opcode color"),
            "b0x00", false
        }
    },
    {
        "b0x7f", {
            QObject::tr("0x7f opcode color"),
            "b0x7f", false
        }
    },
    {
        "b0xff", {
            QObject::tr("0xff opcode color"),
            "b0xff", false
        }
    },
    {
        "math", {
            QObject::tr("arithmetic color (+, -, *, / etc.)"),
            "math", false
        }
    },
    { "bin", { "", QObject::tr("bin"), false } },
    { "btext", { "", QObject::tr("btext"), false } },
    { "push", { QObject::tr("push opcode color"), "push", false } },
    { "pop", { QObject::tr("pop opcode color"), "pop", false } },
    { "crypto", { QObject::tr("Cryptographic color"), "crypto", false } },
    {
        "jmp", {
            QObject::tr("jmp instructions color"),
            "jmp", false
        }
    },
    {
        "cjmp", {
            "",
            "cjmp", false
        }
    },
    {
        "call", {
            QObject::tr("call instructions color (ccall, rcall, call etc)"),
            "call", false
        }
    },
    { "nop", { QObject::tr("nop opcode color"), "nop", false } },
    {
        "ret", {
            QObject::tr("ret opcode color"),
            "ret", false
        }
    },
    {
        "trap", {
            QObject::tr("Color of interrupts"),
            QObject::tr("Interrupts"), false
        }
    },
    { "swi", { QObject::tr("swi opcode color"), "swi", false } },
    { "cmp", { QObject::tr("cmp opcode color"), "cmp", false } },
    { "reg", { QObject::tr("Registers color"), QObject::tr("Register"), false } },
    { "creg", { QObject::tr("Info"), "creg", false } },
    { "num", { QObject::tr("Numeric constants color"), QObject::tr("Numbers"), false } },
    {
        "mov", {
            QObject::tr("mov instructions color (mov, movd, movw etc"),
            QObject::tr("mov"), false
        }
    },
    {
        "func_var", {
            QObject::tr("Function variable color"),
            QObject::tr("Function variable"), false
        }
    },
    {
        "func_var_type", {
            QObject::tr("Function variable (local or argument) type color"),
            QObject::tr("Variable type"), false
        }
    },
    {
        "func_var_addr", {
            QObject::tr("Function variable address color"),
            QObject::tr("Variable address"), false
        }
    },
    { "widget_bg", { "", "widget_bg", false } },
    { "widget_sel", { "", "widget_sel", false } },
    { "ai.read", { "", "ai.read", false } },
    { "ai.write", { "", "ai.write", false } },
    { "ai.exec", { "", "ai.exec", false } },
    { "ai.seq", { "", "ai.seq", false } },
    { "ai.ascii", { "", "ai.ascii", false } },
    { "graph.box", { "", "graph.box", false } },
    { "graph.box2", { "", "graph.box2", false } },
    { "graph.box3", { "", "graph.box3", false } },
    { "graph.box4", { "", "graph.box4", false } },
    {
        "graph.true", {
            QObject::tr("In graph view jump arrow true"),
            QObject::tr("Arrow true"), false
        }
    },
    {
        "graph.false", {
            QObject::tr("In graph view jump arrow false"),
            QObject::tr("Arrow false"), false
        }
    },
    {
        "graph.trufae", {
            QObject::tr("In graph view jump arrow (no condition)"),
            QObject::tr("Arrow"), false
        }
    },
    { "graph.current", { "", "graph.current", false } },
    { "graph.traced", { "", "graph.traced", false } },
    {
        "gui.overview.node", {
            QObject::tr("Background color of Graph Overview's node"),
            QObject::tr("Graph Overview node"), true
        }
    },
    { "gui.cflow", { "", "gui.cflow", true } },
    { "gui.dataoffset", { "", "gui.dataoffset", true } },
    {
        "gui.background", {
            QObject::tr("General background color"),
            QObject::tr("Background"), true
        }
    },
    {
        "gui.alt_background", {
            "",
            QObject::tr("Alt. background"), true
        }
    },
    {
        "gui.disass_selected", {
            QObject::tr("Background of current graph node"),
            QObject::tr("Current graph node"), true
        }
    },
    { "gui.border", { "", "gui.border", true } },

    // TODO: find out different
    {
        "linehl", {
            QObject::tr("Selected line background color"),
            QObject::tr("Line highlight"), true
        }
    },
    {
        "highlight", {
            QObject::tr("Selected line background color"),
            QObject::tr("Line highlight"), true
        }
    },

    // TODO: find out different
    {
        "highlightWord", {
            QObject::tr("Highlighted word text color"),
            QObject::tr("Word higlight"), true
        }
    },
    {
        "wordhl", {
            QObject::tr("Highlighted word text color"),
            QObject::tr("Word higlight"), true
        }
    },

    {
        "gui.main", {
            QObject::tr("Main function color"),
            QObject::tr("Main"), true
        }
    },
    { "gui.imports", { "", "gui.imports", true } },
    { "highlightPC", { "", "highlightPC", true } },
    { "gui.navbar.err", { "", "gui.navbar.err", true } },
    { "gui.navbar.seek", { "", "gui.navbar.seek", true } },
    { "gui.navbar.pc", { "", "gui.navbar.pc", true } },
    { "gui.navbar.sym", { "", "gui.navbar.sym", true } },
    {
        "gui.navbar.code", {
            QObject::tr("Code section color in navigation bar"),
            QObject::tr("Navbar code"), true
        }
    },
    {
        "gui.navbar.empty", {
            QObject::tr("Empty section color in navigation bar"),
            QObject::tr("Navbar empty"), true
        }
    },
    { "gui.breakpoint_background", { "", QObject::tr("Breakpoint background"), true } },
    {
        "gui.tooltip.background", {
            QObject::tr("Background color for tooltips"),
            QObject::tr("Tooltip background"), true
        }
    },
    {
        "gui.tooltip.foreground", {
            QObject::tr("Foregorund color for tooltips"),
            QObject::tr("Tooltip foreground"), true
        }
    }
};

void ColorOptionDelegate::paint(QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    painter->setPen(QPen(Qt::OpaqueMode));
    painter->setBrush(QBrush(backgroundColor));
    painter->drawRect(option.rect);

    QPalette pal;

    QStyleOptionViewItem op = option;

    op.palette = pal;
    op.displayAlignment = Qt::AlignCenter;

    if (option.state & QStyle::State_Selected) {
        QLinearGradient lgrd = QLinearGradient(option.rect.topLeft(), option.rect.bottomRight());
        QColor highlighted = QApplication::palette().highlight().color(),
               highlightedOpaque = highlighted;
        highlightedOpaque.setAlpha(0);

        lgrd.setColorAt(0.00, highlighted);
        lgrd.setColorAt(0.25, highlightedOpaque);
        lgrd.setColorAt(0.75, highlightedOpaque);
        lgrd.setColorAt(1.00, highlighted);
        painter->setBrush(lgrd);
        painter->drawRect(option.rect);
        op.state &= ~ QStyle::State_Selected;
    }

    op.state &= ~ QStyle::State_Editing;
    pal = option.palette;

    QColor txtColor;

    ColorOption co = index.data(Qt::UserRole).value<ColorOption>();
    txtColor = co.color;

    if (optionInfoMap[co.optionName].isUsedStandardTextColor) {
        txtColor = textColor;
    }
    pal.setColor(QPalette::Text, txtColor);

    op.palette = pal;
    QStyledItemDelegate::paint(painter, op, index);
}

void ColorOptionDelegate::setBackgroundColor(const QColor &c)
{
    backgroundColor = c;
}

void ColorOptionDelegate::setTextColor(const QColor &c)
{
    textColor = c;
}

ColorViewButton::ColorViewButton(QWidget *parent) : QFrame (parent)
{
    setLineWidth(3);
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
    setColor(palette().background().color());
    setMaximumWidth(100);
}

void ColorViewButton::setColor(const QColor &c)
{
    setStyleSheet(QString("background-color:%1;").arg(c.name().toLower()));
    repaint();
}

void ColorViewButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    emit clicked();
}

PreferencesListView::PreferencesListView(QWidget *parent) :
    QListView (parent)
{
    setModel(new ColorSettingsModel(static_cast<QObject *>(this)));

    static_cast<ColorSettingsModel *>(this->model())->updateScheme();
    setStandardColors();
}

void PreferencesListView::setStandardColors()
{
    delegate = new ColorOptionDelegate(this);
    ColorSettingsModel *model = static_cast<ColorSettingsModel *>(this->model());

    delegate->setBackgroundColor(model->getBackroundColor());
    delegate->setTextColor(model->getTextColor());
    // I can't free last delegate, but PreferencesListView will delete it,
    // because every delegate is its child.
    setItemDelegate(static_cast<QAbstractItemDelegate *>(delegate));
}

void PreferencesListView::currentChanged(const QModelIndex &current,
                                         const QModelIndex &previous)
{
    emit indexChanged(current);
    QListView::currentChanged(current, previous);
}

ColorSchemePrefWidget::ColorSchemePrefWidget(QWidget *parent) : QWidget (parent),
    ui (new Ui::ColorSchemePrefWidget), isEditable (false)
{
    ui->setupUi(this);
    connect(ui->colorViewFore, &ColorViewButton::clicked, this, &ColorSchemePrefWidget::newColor);
    connect(ui->preferencesListView, &PreferencesListView::indexChanged, this,
            &ColorSchemePrefWidget::indexChanged);
    connect(ui->preferencesListView, &PreferencesListView::indexChanged, [this](const QModelIndex & i) {
        ui->infoBoard->setText(optionInfoMap[i.data(Qt::UserRole).value<ColorOption>().optionName].info);
    });
}

ColorSchemePrefWidget::~ColorSchemePrefWidget()
{
    apply();
    delete ui;
}

void ColorSchemePrefWidget::apply()
{
    if (!isEditable) {
        return;
    }
    ColorSettingsModel *model = static_cast<ColorSettingsModel *>(ui->preferencesListView->model());

    QString scheme = "";
    ColorOption curr;
    QMap<QString, QColor> cutterSpecific = ColorSchemeFileWorker().getCutterSpecific();

    for (int i = 0; i < model->rowCount(); i++) {
        curr = model->data(model->index(i), Qt::UserRole).value<ColorOption>();
        if (cutterSpecific.contains(curr.optionName)) {
            scheme += "#~";
        } else {
            scheme += "ec ";
        }
        scheme += curr.optionName + " rgb:" + curr.color.name().remove(QLatin1Char('#')).toLower() + "\n";
    }
    ColorSchemeFileWorker().save(scheme, Config()->getColorTheme());
    Config()->setColorTheme(Config()->getColorTheme());
}

void ColorSchemePrefWidget::newColor()
{
    if (ui->preferencesListView->currentIndex().row() == -1 || !isEditable) {
        return;
    }

    ColorOption currCO = ui->preferencesListView->model()->data(ui->preferencesListView->currentIndex(),
                                                                Qt::UserRole).value<ColorOption>();
    QColorDialog d(currCO.color, this);
    if (QDialog::Accepted != d.exec())
        return;

    static_cast<ColorSettingsModel *>(ui->preferencesListView->model())->setColor(currCO.optionName,
                                                                                  d.selectedColor());
    ui->preferencesListView->setStandardColors();
    if (ui->preferencesListView->model()->rowCount())
        indexChanged(ui->preferencesListView->selectionModel()->selectedIndexes().first());
}

void ColorSchemePrefWidget::indexChanged(const QModelIndex &ni)
{
    ui->colorViewFore->setColor(ni.data(Qt::UserRole).value<ColorOption>().color);
}

void ColorSchemePrefWidget::updateSchemeFromConfig()
{
    isEditable = ColorSchemeFileWorker().isCustomScheme(Config()->getColorTheme());
    static_cast<ColorSettingsModel *>(ui->preferencesListView->model())->updateScheme();
}

ColorSettingsModel::ColorSettingsModel(QObject *parent) : QAbstractListModel (parent) { }

QVariant ColorSettingsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
      return QVariant();
    }
    
    if (index.row() < 0 || index.row() >= m_data.size()) {
      return QVariant();
    }
    
    if (role == Qt::DisplayRole) {
        return QVariant::fromValue(m_data.at(index.row()).displayingText);
    }

    if (role == Qt::UserRole) {
        return QVariant::fromValue(m_data.at(index.row()));
    }

    return QVariant();
}

void ColorSettingsModel::setColor(const QString &option, const QColor &color)
{
    int row = 0;
    for (auto &it : m_data) {
        if (it.optionName == option) {
            it.color = color;
            emit dataChanged(index(row), index(row));
            return;
        }
        row++;
    }
}

QColor ColorSettingsModel::getBackroundColor() const
{
    if (!ColorSchemeFileWorker().isCustomScheme(Config()->getColorTheme())) {
        return Config()->getColor(standardBackgroundOptionName);
    }

    for (auto &it : m_data) {
        if (it.optionName == standardBackgroundOptionName) {
            return it.color;
        }
    }
    return QColor();
}

QColor ColorSettingsModel::getTextColor() const
{
    if (!ColorSchemeFileWorker().isCustomScheme(Config()->getColorTheme())) {
        return Config()->getColor(standardTextOptionName);
    }

    for (auto &it : m_data) {
        if (it.optionName == standardTextOptionName) {
            return it.color;
        }
    }
    return QColor();
}

void ColorSettingsModel::updateScheme()
{
    m_data.clear();
    QJsonObject obj = Core()->cmdj("ecj").object();

    m_data.reserve(obj.size());
    for (auto &it : obj.keys()) {
        QJsonArray rgb = obj[it].toArray();
        m_data.push_back({it, optionInfoMap[it].displayingtext, QColor(rgb[0].toInt(), rgb[1].toInt(), rgb[2].toInt())});
    }

    QMap<QString, QColor> cutterSpecific = ColorSchemeFileWorker().getCutterSpecific();
    for (auto &it : cutterSpecific.keys()) {
        m_data.push_back({it, optionInfoMap[it].displayingtext, cutterSpecific[it]});
    }

    qobject_cast<PreferencesListView *>(parent())->setStandardColors();
}
