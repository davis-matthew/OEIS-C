/*
 * A188999  Bi-unitary sigma: sum of the bi-unitary divisors of n.
 * https://oeis.org/A188999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = sum of bi‑unitary divisors of n (OEIS A188999)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* handle n = 0 (outside offset) and n = 1 explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* factor n */
    uint64_t nn = (uint64_t)n;           /* cast for the utils function   */
    uint64_t primes[64];                 /* holds the distinct primes     */
    uint64_t exps[64];                   /* holds the corresponding exponents */
    size_t   cnt   = 0;                  /* number of distinct primes     */
    factor_u64(nn, primes, exps, &cnt);

    /* accumulation of the multiplicative function */
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t sum  = (seq_t)1;          /* contribution of p^0 = 1      */
        seq_t term = (seq_t)1;          /* term = p^k                   */
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        for (uint64_t k = 1; k <= e; ++k)
        {
            term *= (seq_t)p;          /* term = p^k                   */
            sum  += term;              /* add p^k to the sum           */
            if ((e & 1U) == 0U && k == e / 2) /* e even and k == e/2   */
                sum -= term;           /* subtract p^(e/2)              */
        }
        result *= sum;                  /* multiply the prime contributions */
    }

    return result;
}
