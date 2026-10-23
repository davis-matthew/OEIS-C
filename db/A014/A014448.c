/*
 * A014448  Even Lucas numbers: a(n) = L(3*n).
 * https://oeis.org/A014448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-2), a(0)=2, a(1)=4; (Lucas numbers L(3n))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)4;

    seq_t prev2 = (seq_t)2; /* a(0) */
    seq_t prev1 = (seq_t)4; /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t cur = prev1 * (seq_t)4 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
