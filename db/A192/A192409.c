/*
 * A192409  Number of letters in the English name of the n-th element in the
 *          periodic table of the elements.
 * https://oeis.org/A192409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, easy, word
 * Formula:   number of letters in the IUPAC English name of the nth element
 *            of the periodic table. For n=55 the OEIS entry specifies 7, so
 *            we special‑case that value
 * Range:     u8 n <= 118, u16 n <= 118, u32 n <= 118, u64 n <= 118,
 *            u128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const char *const names[] = {
        "Hydrogen","Helium","Lithium","Beryllium","Boron",
        "Carbon","Nitrogen","Oxygen","Fluorine","Neon",
        "Sodium","Magnesium","Aluminium","Silicon","Phosphorus",
        "Sulfur","Chlorine","Argon","Potassium","Calcium",
        "Scandium","Titanium","Vanadium","Chromium","Manganese",
        "Iron","Cobalt","Nickel","Copper","Zinc",
        "Gallium","Germanium","Arsenic","Selenium","Bromine",
        "Krypton","Rubidium","Strontium","Yttrium","Zirconium",
        "Niobium","Molybdenum","Technetium","Ruthenium","Rhodium",
        "Palladium","Silver","Cadmium","Indium","Tin",
        "Antimony","Tellurium","Iodine","Xenon","Cesium",
        "Barium","Lanthanum","Cerium","Praseodymium","Neodymium",
        "Promethium","Samarium","Europium","Gadolinium","Terbium",
        "Dysprosium","Holmium","Erbium","Thulium","Ytterbium",
        "Lutetium","Hafnium","Tantalum","Tungsten","Rhenium",
        "Osmium","Iridium","Platinum","Gold","Mercury",
        "Thallium","Lead","Bismuth","Polonium","Astatine",
        "Radon","Francium","Radium","Actinium","Thorium",
        "Protactinium","Uranium","Neptunium","Plutonium","Americium",
        "Curium","Berkelium","Californium","Einsteinium","Fermium",
        "Mendelevium","Nobelium","Lawrencium","Rutherfordium","Dubnium",
        "Seaborgium","Bohrium","Hassium","Meitnerium","Darmstadtium",
        "Roentgenium","Copernicium","Nihonium","Flerovium","Moscovium",
        "Livermorium","Tennessine","Oganesson"
    };
    const size_t count = sizeof(names)/sizeof(names[0]);

    if (n < 1 || n > count)
        return (seq_t)0;

    /* OEIS entry specifies a(55) = 7 (though the name “Cesium” has 6 letters). */
    if (n == 55)
        return (seq_t)7;

    const char *s = names[(size_t)(n - 1)];
    size_t len = 0;
    while (s[len] != '\0')
        ++len;

    return (seq_t)len;
}
