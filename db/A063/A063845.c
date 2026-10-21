/*
 * A063845  a(n) = sigma(usigma(n)), where usigma(n) is the sum of unitary
 *          divisors of n (A034448) and sigma(n) is the sum of the divisors
 *          (A000203).
 * https://oeis.org/A063845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(usigma(n)) where usigma(n) = sum of unitary
 *            divisors of n and sigma(n) = sum of all divisors of n
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   usigma_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/usigma_uint64.h"

/* Compute σ(n) for n ≤ 2^64 - 1 using trial division. */
static seq_t sigma_uint64(uint64_t n)
{
    seq_t result = 1;
    uint64_t tmp   = n;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            unsigned int exp = 0;
            uint64_t p_pow   = 1; /* p^exp */
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
                p_pow *= p;
            }
            /* term = (p^(exp+1) - 1) / (p - 1) */
            uint64_t numerator = p_pow * p - 1; /* p^(exp+1) - 1 */
            uint64_t denom     = p - 1;
            seq_t term = (seq_t)(numerator / denom);
            result *= term;
        }
    }
    if (tmp > 1) {
        /* Remaining prime factor: term = 1 + tmp */
        result *= ((seq_t)tmp + 1);
    }
    return result;
}

/* Sequence function: a(n) = σ(usigma(n)) */
seq_t a(idx_t n)
{
    /* n ≥ 1 by specification */
    uint64_t nn  = (uint64_t)n;
    seq_t us = usigma_uint64(nn);
    /* us fits into uint64_t for the range of n we support */
    uint64_t us_val = (uint64_t)us;
    return sigma_uint64(us_val);
}
