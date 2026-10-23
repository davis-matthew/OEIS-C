/*
 * A041330  Numerators of continued fraction convergents to sqrt(179).
 * https://oeis.org/A041330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 19, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence for numerators of convergents to sqrt(179):
 *   p_{-2} = 0,  p_{-1} = 1,
 *   p_n   = c_n * p_{n-1} + p_{n-2}   (n ≥ 0)
 * where the coefficient sequence is
 *   c_0 = 13,  and for n > 0
 *   c_n = {2,1,1,1,3,5,13,5,3,1,1,1,2,26}   (repeating every 14 terms).
 *
 * Initial values:
 *   a(0) = 13
 *   a(1) = 27
 *   a(2) = 40
 *   ...
 */
seq_t a(idx_t n) {
    /* static cache for computed numerators */
    static seq_t *p = NULL;
    static idx_t len = 0;      /* number of terms stored (next to compute is len) */
    static size_t cap = 0;     /* current capacity of p[] */

    /* coefficient table: c_0 ... c_14 */
    static const idx_t coeff[15] = {
        13,  /* c_0 */
        2,1,1,1,3,5,13,5,3,1,1,1,2,26   /* c_1 .. c_14 */
    };

    /* ensure array has room for n+1 terms */
    if (cap <= n) {
        size_t newcap = n + 1;
        if (newcap < 32) newcap = 32;   /* grow modestly first */
        p = (seq_t *)realloc(p, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* compute missing terms */
    if (len == 0) {
        /* initialise first term */
        p[0] = (seq_t)coeff[0];   /* 13 */
        len = 1;
    }
    for (idx_t i = len; i <= n; ++i) {
        /* coefficient for this index i */
        idx_t cidx = ((i == 0) ? 0 : ((i - 1) % 14) + 1);
        seq_t coef = (seq_t)coeff[cidx];

        if (i == 1) {
            /* special case: use p[-1] = 1 */
            p[i] = coef * p[i - 1] + (seq_t)1;
        } else {
            p[i] = coef * p[i - 1] + p[i - 2];
        }
    }
    len = n + 1;

    return p[n];
}
