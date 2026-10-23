/*
 * A038548  Number of divisors of n that are at most sqrt(n).
 * https://oeis.org/A038548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ceil(d(n)/2) where d(n) = number of divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation counts divisors using factor_u64 from the utils
   library and returns the smallest integer ≥ d(n)/2, i.e. the number
   of divisors ≤ sqrt(n).  It works for any idx_t ≥ 1. */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

seq_t a(idx_t n)
{
    /* factor_u64 expects the prime factors and exponents placed in
       user-provided arrays.  For a 64‑bit integer, at most 16 distinct
       primes can occur (product of the first 16 primes already exceeds
       2^64).  We therefore allocate a modest fixed size array. */
    static const size_t MAX_FACTS = 20;
    uint64_t pr[MAX_FACTS];
    uint64_t ex[MAX_FACTS];
    size_t cnt;

    /* Decompose n into primes and exponents */
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Compute d(n) = ∏ (ex[i] + 1) */
    uint64_t d = 1;
    for (size_t i = 0; i < cnt; ++i)
        d = d * (ex[i] + 1);

    /* a(n) = ceil(d/2) = (d + 1) / 2 */
    return (seq_t)((d + 1) / 2);
}
