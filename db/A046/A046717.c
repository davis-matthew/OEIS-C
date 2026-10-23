/*
 * A046717  a(n) = 2*a(n-1) + 3*a(n-2), a(0) = a(1) = 1.
 * https://oeis.org/A046717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2), a(0) = a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)1; /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term1 = mul_small(a1, 2);
        seq_t term2 = mul_small(a0, 3);
        seq_t a2 = term1 + term2;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
