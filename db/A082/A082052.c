/*
 * A082052  Sum of divisors of n that are not of the form 4k+1.
 * https://oeis.org/A082052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of all divisors d of n with d % 4 != 1
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_div_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute a(n) for all n ≥ 1.
 *  The routine factors n by trial division, enumerates all its divisors,
 *  and accumulates those whose remainder modulo 4 is not 1.
 *
 *  It works with any seq_t type that supports +, +=, *, and % with
 *  integral constants (including arbitrary‑precision integer classes).
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/dfs_div_sum.h"

/* Maximum number of distinct prime factors for a 64‑bit integer.
 * 20 is ample for all 64‑bit numbers. */
#define MAX_PRIME_FACTORS 20

seq_t
a(idx_t n)
{
    /* Ensure we operate on a 64‑bit value for factorisation */
    uint64_t temp = (uint64_t)n;

    /* Arrays to hold the prime factorisation */
    uint64_t primes[MAX_PRIME_FACTORS];
    unsigned exponents[MAX_PRIME_FACTORS];
    unsigned num_factors = 0;

    /* Trial division up to sqrt(temp) */
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (num_factors < MAX_PRIME_FACTORS) {
                primes[num_factors]   = p;
                exponents[num_factors] = e;
                ++num_factors;
            }
        }
    }
    if (temp > 1) {          /* Remaining prime factor > sqrt(original n) */
        if (num_factors < MAX_PRIME_FACTORS) {
            primes[num_factors]   = temp;
            exponents[num_factors] = 1;
            ++num_factors;
        }
    }

    seq_t sum = (seq_t)0;
    dfs_div_sum(0, (seq_t)1, num_factors, primes, exponents, &sum);
    return sum;
}
