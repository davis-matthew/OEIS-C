/*
 * A132609  Antidiagonal sum of table A072590(n,k) = n^(k-1)*k^(n-1) for n>=1.
 * https://oeis.org/A132609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n-k+1)^(k-1) * k^(n-k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Guard against n == 0, though the sequence is defined for n>=1. */
    if (n == 0)
        return (seq_t)0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute (n - k + 1)^(k - 1) */
        seq_t base1 = (seq_t)(n - k + 1);
        seq_t exp1  = (seq_t)(k - 1);
        seq_t term1 = big_pow(base1, exp1);

        /* Compute k^(n - k) */
        seq_t base2 = (seq_t)k;
        seq_t exp2  = (seq_t)(n - k);
        seq_t term2 = big_pow(base2, exp2);

        /* Multiply the two powers and add to the running total */
        seq_t term = seq_mul(term1, term2);
        sum += term;
    }
    return sum;
}
