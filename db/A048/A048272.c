/*
 * A048272  Number of odd divisors of n minus number of even divisors of n.
 * https://oeis.org/A048272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, nice, mult
 * Formula:   a(n) = number of odd divisors of n minus number of even divisors
 *            of n. * * The sequence is multiplicative with * a(2^e) = 1 - e,
 *            * a(p^e) = 1 + e for odd prime p. * This implementation factors
 *            n and multiplies the corresponding terms
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 not required, but safety */
    if (n == 0) return 0;  /* undefined, but protect against misuse */

    /* Temporary arrays for prime factors and exponents.
       For a 64‑bit number the maximum number of distinct prime factors is 16,
       so allocate a small constant array. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2) {
            result *= ((seq_t)1 - (seq_t)exps[i]);   /* a(2^e) = 1 - e */
        } else {
            result *= ((seq_t)1 + (seq_t)exps[i]);   /* a(p^e) = 1 + e for p > 2 */
        }
    }

    return result;
}
