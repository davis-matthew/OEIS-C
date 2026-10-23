/*
 * A163062  a(n) = ((3+sqrt(5))*(1+sqrt(5))^n + (3-sqrt(5))*(1-sqrt(5))^n)/2.
 * https://oeis.org/A163062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2), a(0)=3, a(1)=8
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)8;

    seq_t a0 = (seq_t)3;   /* a(0) */
    seq_t a1 = (seq_t)8;   /* a(1) */
    seq_t ai;

    for (idx_t i = 2; i <= n; ++i) {
        ai = 2 * a1 + 4 * a0;
        a0 = a1;
        a1 = ai;
    }
    return ai;
}
