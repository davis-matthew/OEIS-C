/*
 * A066620  Number of unordered triples of distinct pairwise coprime divisors
 *          of n.
 * https://oeis.org/A066620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (tau(n^3) - 3*tau(n) + 2) / 6
 * Range:     u8 n <= 4619, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau(m) is the number of divisors of m.  */
/* This implementation uses the lib factor_u64 helper.  */
/* It is O(#prime_factors(n)) per call.  */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factor n */
    /* n may be large but factor_u64 accepts uint64_t */
    uint64_t m = (uint64_t)n;
    /* A number up to 2^64 cannot have more than 64 distinct prime factors */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(m, primes, exps, &cnt);

    /* Compute tau(n) and tau(n^3) */
    seq_t tau_n  = 1;
    seq_t tau_n3 = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t e  = (seq_t)exps[i];
        tau_n  *= (e + 1);
        tau_n3 *= (3*e + 1);
    }

    /* Apply the formula */
    seq_t numerator = tau_n3 - 3*tau_n + 2;
    seq_t result = numerator / 6;   /* guaranteed integer division */

    return result;
}
