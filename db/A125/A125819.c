/*
 * A125819  a(n) = ((1 + 7*sqrt(2))^n - (1 - 7*sqrt(2))^n)/(14*sqrt(2)).
 * https://oeis.org/A125819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 97*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t a2;

    for (idx_t i = 2; i <= n; ++i)
    {
        a2 = mul_small(a1, 2) + mul_small(a0, 97);
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
