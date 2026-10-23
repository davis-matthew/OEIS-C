/*
 * A101822  Expansion of 1/(1-x-2*x^2-3*x^3).
 * https://oeis.org/A101822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 3*a(n-3), with a(0)=1, a(1)=1, a(2)=3
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)3;   /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t ai = a2 + a1 * (seq_t)2 + a0 * (seq_t)3;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
