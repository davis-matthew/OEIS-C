/*
 * A180411  Sum of the semiprime divisors (with repetition) of the n-th number
 *          with two or more distinct semiprime divisors.
 * https://oeis.org/A180411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the semiprime divisors (with multiplicity) * of
 *            the n‑th integer that has at least two distinct * semiprime
 *            divisors. Computation is performed by * iterating over
 *            successive integers and accumulating * the required sums when
 *            the condition is satisfied
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation works for all supported seq_t / idx_t
 * definitions, including arbitrary‑precision seq_t in C++17 builds,
 * and it does not use dynamic allocation beyond the utilities. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute the a(n) value.  n >= 1; n == 0 returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    idx_t found = 0;               /* how many qualifying numbers found */
    uint64_t candidate = 2;        /* candidate integer to test */

    /* Temporary storage for prime factorisation */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    while (found < n) {
        /* Factorise candidate */
        factor_u64(candidate, primes, exps, &cnt);

        /* Count distinct primes and those with exponent >= 2 */
        size_t k = cnt;
        size_t ge2 = 0;
        for (size_t i = 0; i < k; ++i)
            if (exps[i] >= 2) ++ge2;

        /* Number of distinct semiprime products */
        idx_t distinct = (idx_t)(k * (k - 1) / 2) + (idx_t)ge2;

        if (distinct >= 2) {            /* qualifies */
            ++found;
            if (found == n) {          /* answer for this n */
                seq_t sum = 0;

                /* Same‑prime pairs: C(e_i, 2) * p_i^2 */
                for (size_t i = 0; i < k; ++i) {
                    if (exps[i] >= 2) {
                        seq_t comb = (seq_t)exps[i] * (seq_t)(exps[i] - 1) / 2;
                        seq_t prod = (seq_t)primes[i] * (seq_t)primes[i];
                        sum += comb * prod;
                    }
                }

                /* Different‑prime pairs: e_i * e_j * p_i * p_j */
                for (size_t i = 0; i < k; ++i) {
                    for (size_t j = i + 1; j < k; ++j) {
                        seq_t mult = (seq_t)exps[i] * (seq_t)exps[j];
                        seq_t prod = (seq_t)primes[i] * (seq_t)primes[j];
                        sum += mult * prod;
                    }
                }

                return sum;
            }
        }

        ++candidate;
    }

    /* Should never reach here */
    return 0;
}
