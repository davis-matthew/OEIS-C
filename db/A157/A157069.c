/*
 * A157069  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 40.
 * https://oeis.org/A157069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} binomial(n+1,i) * binomial(19,i-1) *
 *            binomial(n-i+20,20)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 20, u128 n <= 74,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts sequences of length n+1 with sum zero and sum of absolute values 40. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    const idx_t k = 20;   /* sum of absolute values divided by 2 */
    seq_t total = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t b1 = binomial(n + 1, i);          /* binomial(n+1, i) */
        seq_t b2 = binomial(k - 1, i - 1);      /* binomial(19, i-1) */
        seq_t b3 = binomial(n - i + k, k);      /* binomial(n-i+20, 20) */
        total += b1 * b2 * b3;
    }
    return total;
}
