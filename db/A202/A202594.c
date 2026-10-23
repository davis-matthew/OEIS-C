/*
 * A202594  Number of (n+2) X 3 binary arrays avoiding patterns 001 and 000 in
 *          rows and columns.
 * https://oeis.org/A202594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +7*a(n-2) +2*a(n-3) -a(n-4) (empirical, n≥5)
 *            Initial values: a(1)=126, a(2)=510, a(3)=1968, a(4)=7722
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 126;
    if (n == 2) return 510;
    if (n == 3) return 1968;
    if (n == 4) return 7722;

    seq_t a0 = 126;   /* a(1) */
    seq_t a1 = 510;   /* a(2) */
    seq_t a2 = 1968;  /* a(3) */
    seq_t a3 = 7722;  /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t ai = (seq_t)2 * a3 + (seq_t)7 * a2 + (seq_t)2 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
