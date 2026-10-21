/*
 * A227908  Number of ways to write 2*n = p + q with p, q and (p-1)^2 + q^2
 *          all prime.
 * https://oeis.org/A227908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (p,q) of primes with p + q = 2*n
 *            and (p-1)^2 + q^2 also prime
 * Range:     u8 n <= 11, u16 n <= 602, u32 n <= 602, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the helper functions:
   - build_primes_upto(idx_t limit, uint64_t **primes, size_t *cnt)
   - is_prime_big(seq_t n)
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

/* Static cache of primes up to a certain limit */
static idx_t prime_limit = 0;
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

/* Ensure the prime list contains all primes up to 'limit' */
static void ensure_prime_cache(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* free previous list if any */
    if (prime_list) free(prime_list);

    /* build new prime list up to limit */
    build_primes_upto(limit, &prime_list, &prime_count);

    prime_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t target = 2 * n;               /* p + q = target */
    /* No need to precompute primes larger than target for q checks,
       but we need them for p candidates. */
    ensure_prime_cache(target);

    seq_t count = 0;

    /* Iterate over all primes p <= target-2 (q >= 2). */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > target - 2) break;

        uint64_t q = target - p;
        /* q must be prime */
        if (!is_prime_big((seq_t)q)) continue;

        /* Compute (p-1)^2 + q^2   using seq_t to avoid overflow */
        seq_t a_part = (seq_t)(p - 1);
        a_part = a_part * a_part;

        seq_t b_part = (seq_t)q;
        b_part = b_part * b_part;

        seq_t sum = a_part + b_part;

        if (is_prime_big(sum)) ++count;
    }

    return count;
}
