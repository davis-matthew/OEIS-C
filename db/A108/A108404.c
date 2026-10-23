/*
 * A108404  Expansion of (1-4x)/(1-8x+11x^2).
 * https://oeis.org/A108404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=4, a(n)=8*a(n-1)-11*a(n-2) for n>=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* Iterate recurrence up to n */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)4;   /* a(1) */
    seq_t ai = (seq_t)0;   /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        ai = (seq_t)8 * a1 - (seq_t)11 * a0;
        a0 = a1;
        a1 = ai;
    }
    return ai;
}
