/*
 * A085079  Largest number with the prime signature of n using prime divisors
 *          of n.
 * https://oeis.org/A085079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ m | prime_signature(m) = prime_signature(n) }
 * Range:     u8 n <= 51, u16 n <= 271, u32 n <= 2367, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, sort_uint64_asc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest number with the same multiset of prime exponents
   as n, using the prime divisors of n.  For each prime factor p^e of n,
   e is assigned to the largest remaining prime factor to maximize the result. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/big_pow.h"      /* big_pow(seq_t base, seq_t exp) */
#include "../utils/sort_uint64_asc.h"

seq_t a(idx_t n)
{
    /* n is the index, which we use as the number to factor. */
    uint64_t prime_factors[64];
    uint64_t exponents[64];
    size_t     count = 0;

    /* Factor n (n is 1 or greater).  For n=1, factor_u64 produces count=0. */
    factor_u64((uint64_t)n, prime_factors, exponents, &count);

    if (count == 0)
        return (seq_t)1;        /* a(1) = 1 */

    /* Sort primes ascending and exponents ascending.
       This ensures the largest exponent gets paired with the largest prime. */
    sort_uint64_asc(prime_factors, count);
    sort_uint64_asc(exponents, count);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < count; ++i)
    {
        seq_t base = (seq_t)prime_factors[i];
        seq_t exp  = (seq_t)exponents[i];
        result *= big_pow(base, exp);
    }
    return result;
}
