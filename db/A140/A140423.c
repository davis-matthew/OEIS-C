/*
 * A140423  a(n) = Sum_{k=1..n} k^(2^(n+1-k)).
 * https://oeis.org/A140423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^(2^(n+1-k))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/pow2.h"

/* Computes a(n) for n >= 1. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* exponent = 2^(n+1-k) */
        idx_t exp_idx = n + 1 - k;
        seq_t exp = pow2(exp_idx);         /* 2^exp_idx */
        seq_t term = big_pow((seq_t)k, exp); /* k^exp */
        sum += term;
    }
    return sum;
}
