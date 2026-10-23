/*
 * A122512  Expansion of 1/(1 - x^2 - 2 x^3 + x^4).
 * https://oeis.org/A122512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=1, a(3)=2 for n >= 4: a(n) = a(n-2) +
 *            2*a(n-3) - a(n-4) This is the expansion of 1/(1 - x^2 - 2x^3 +
 *            x^4)
 * Range:     u8 n <= 18, u16 n <= 36, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* a0,a1,a2,a3 hold a(n-4), a(n-3), a(n-2), a(n-1) respectively */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)0;   /* a(1) */
    seq_t a2 = (seq_t)1;   /* a(2) */
    seq_t a3 = (seq_t)2;   /* a(3) */
    seq_t term = 0;

    for (idx_t k = 4; k <= n; ++k) {
        term = a2;            /* a(n-2) */
        term += (seq_t)2 * a1;/* 2*a(n-3) */
        term -= a0;           /* -a(n-4) */

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = term;
    }

    return term;
}
