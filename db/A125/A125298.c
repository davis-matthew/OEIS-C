/*
 * A125298  Atomic numbers of elements having a single letter chemical symbol.
 * https://oeis.org/A125298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, word
 * Formula:   a(n) is the atomic number of the nth element whose chemical
 *            symbol has length 1
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static const char * const _element_symbols[119] = {
    "", /* 0 unused, elements start at 1 */
    "H",  "He","Li","Be","B","C","N","O","F","Ne",
    "Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca",
    "Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn",
    "Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr",
    "Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn",
    "Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd",
    "Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb",
    "Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg",
    "Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th",
    "Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm",
    "Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","Ds",
    "Rg","Cn","Nh","Fl","Mc","Lv","Ts","Og"
};

seq_t a(idx_t n)
{
    /* a(1) = 1, a(2) = 5, ..., a(14) = 92; undefined for n > 14 (return 0). */
    if (n == 0)
        return (seq_t)0;

    /* Build the list of atomic numbers with single-letter symbols only once. */
    static uint64_t _single_atomic_numbers[14] = {0};
    static size_t _single_count = 0;

    if (_single_count == 0) {
        for (idx_t i = 1; i <= 118; ++i) {
            /* Check if the symbol has exactly one character */
            if (strlen(_element_symbols[i]) == 1) {
                if (_single_count < 14) {
                    _single_atomic_numbers[_single_count] = (uint64_t)i;
                }
                ++_single_count;
            }
        }
    }

    if (n <= _single_count)
        return (seq_t)_single_atomic_numbers[n - 1];
    return (seq_t)0;
}
