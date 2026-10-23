/*
 * A180011  a(n) = smallest k > 1 such that n and k*n have the same digit sum.
 * https://oeis.org/A180011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest k>1 such that digit_sum(n) == digit_sum(k*n)
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* If n is not divisible by 3, the definition guarantees a(n) = 10. */
    if (n % 3u != 0u)
        return (seq_t)10;

    seq_t s_n   = (seq_t)n;
    seq_t sum_n = digit_sum(s_n);

    /* Search for the smallest k > 1 (k <= 10) that preserves the digit sum. */
    for (seq_t k = 2; k <= 10; ++k)
    {
        seq_t prod = s_n * k;
        if (digit_sum(prod) == sum_n)
            return k;
    }

    /* By construction this point is unreachable, but return 10 as a fallback. */
    return (seq_t)10;
}
