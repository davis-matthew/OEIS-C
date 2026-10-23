/*
 * A126061  Number of letters in symbol for n-th element in periodic table.
 * https://oeis.org/A126061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = number of letters in the chemical symbol of element n
 * Range:     u8 n <= 118, u16 n <= 118, u32 n <= 118, u64 n <= 118,
 *            u128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Element symbols up to Og (118) */
    static const char * const symbols[] = {
        "H","He","Li","Be","B","C","N","O","F","Ne",
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
    static const size_t num_symbols = sizeof(symbols) / sizeof(symbols[0]);

    if ((size_t)n > num_symbols) return 0;   /* undefined element */

    const char *s = symbols[n-1];
    size_t len = 0;
    while (*s++) len++;

    return (seq_t)len;
}
