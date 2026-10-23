/*
 * A152160  List of atomic numbers of the 118 permanently named chemical
 *          elements, sorted in alphabetical order according to their symbols.
 * https://oeis.org/A152160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, word, nonn
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 118, u32 n <= 118, u64 n <= 118,
 *            u128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = atomic number of the element whose symbol is the n‑th in
   alphabetical order of the 118 permanent elements. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct Element {
    const char *sym;
    seq_t num;
};

/* All permanent elements with their symbols and atomic numbers. */
static const struct Element elements[118] = {
    {"H",   1},{"He",  2},{"Li",  3},{"Be",  4},{"B",   5},
    {"C",   6},{"N",   7},{"O",   8},{"F",   9},{"Ne", 10},
    {"Na", 11},{"Mg", 12},{"Al", 13},{"Si", 14},{"P",  15},
    {"S",  16},{"Cl", 17},{"Ar", 18},{"K", 19},{"Ca", 20},
    {"Sc", 21},{"Ti", 22},{"V",  23},{"Cr", 24},{"Mn", 25},
    {"Fe", 26},{"Co", 27},{"Ni", 28},{"Cu", 29},{"Zn", 30},
    {"Ga", 31},{"Ge", 32},{"As", 33},{"Se", 34},{"Br", 35},
    {"Kr", 36},{"Rb", 37},{"Sr", 38},{"Y", 39},{"Zr", 40},
    {"Nb", 41},{"Mo", 42},{"Tc", 43},{"Ru", 44},{"Rh", 45},
    {"Pd", 46},{"Ag", 47},{"Cd", 48},{"In", 49},{"Sn", 50},
    {"Sb", 51},{"Te", 52},{"I",  53},{"Xe", 54},{"Cs", 55},
    {"Ba", 56},{"La", 57},{"Ce", 58},{"Pr", 59},{"Nd", 60},
    {"Pm", 61},{"Sm", 62},{"Eu", 63},{"Gd", 64},{"Tb", 65},
    {"Dy", 66},{"Ho", 67},{"Er", 68},{"Tm", 69},{"Yb", 70},
    {"Lu", 71},{"Hf", 72},{"Ta", 73},{"W",  74},{"Re", 75},
    {"Os", 76},{"Ir", 77},{"Pt", 78},{"Au", 79},{"Hg", 80},
    {"Tl", 81},{"Pb", 82},{"Bi", 83},{"Po", 84},{"At", 85},
    {"Rn", 86},{"Fr", 87},{"Ra", 88},{"Ac", 89},{"Th", 90},
    {"Pa", 91},{"U",  92},{"Np", 93},{"Pu", 94},{"Am", 95},
    {"Cm", 96},{"Bk", 97},{"Cf", 98},{"Es", 99},{"Fm",100},
    {"Md",101},{"No",102},{"Lr",103},{"Rf",104},{"Db",105},
    {"Sg",106},{"Bh",107},{"Hs",108},{"Mt",109},{"Ds",110},
    {"Rg",111},{"Cn",112},{"Nh",113},{"Fl",114},{"Mc",115},
    {"Lv",116},{"Ts",117},{"Og",118}
};

static seq_t atoms_sorted[118];
static bool init_done = false;

/* Comparison function for qsort by symbol. */
static int cmp_elem(const void *a, const void *b)
{
    const struct Element *ea = (const struct Element *)a;
    const struct Element *eb = (const struct Element *)b;
    return strcmp(ea->sym, eb->sym);
}

/* One‑time initialization: sort symbols alphabetically and cache atomic
   numbers in atoms_sorted. */
static void init_sequence(void)
{
    struct Element tmp[118];
    size_t i;
    for (i = 0; i < 118; ++i)
        tmp[i] = elements[i];
    qsort(tmp, 118, sizeof(tmp[0]), cmp_elem);
    for (i = 0; i < 118; ++i)
        atoms_sorted[i] = tmp[i].num;
    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done)
        init_sequence();
    if (n == 0 || n > 118)
        return 0;
    return atoms_sorted[n - 1];
}
