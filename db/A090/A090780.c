/*
 * A090780  a(n) = n*Product_{p prime, p|n} (p - 1)/2.
 * https://oeis.org/A090780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n*Product_{p|n} (p-1)/2
 * Range:     u8 n <= 16, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 1. Uses prime factorization via the
   provided factor_u64 helper and combines the distinct prime
   factors multiplicatively.  The result fits into seq_t without
   overflow as seq_t may be arbitrarily large. */
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* factor the integer n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* start with n */
    seq_t result = (seq_t)n;

    /* multiply by (p-1) for each distinct prime factor */
    for (size_t i = 0; i < cnt; ++i)
        result = result * ((seq_t)primes[i] - 1);

    /* divide by 2^cnt, i.e. shift right by cnt bits if possible */
    seq_t pow2 = 1;
    for (size_t i = 0; i < cnt; ++i)
        pow2 = pow2 * 2;
    result = result / pow2;

    return result;
}
