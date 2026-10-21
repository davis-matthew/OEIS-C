/*
 * A214727  a(n) = a(n-1) + a(n-2) + a(n-3) with a(0) = 1, a(1) = a(2) = 2.
 * https://oeis.org/A214727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = a(2) = 2 a(n) = a(n-1) + a(n-2) + a(n-3) for n
 *            ≥ 3 (recurrence with order‑3 linear constant coefficients)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* just for completeness; seq_t/idx_t supplied by the build */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1 || n == 2) {
        return (seq_t)2;
    }

    /* iterate from 3 up to n computing only the last three values */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)2;   /* a(1) */
    seq_t a2 = (seq_t)2;   /* a(2) */
    seq_t ai;
    for (idx_t i = 3; i <= n; ++i) {
        ai = a0 + a1 + a2;   /* a(i) = a(i-1)+a(i-2)+a(i-3) */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;   /* a(n) */
}
