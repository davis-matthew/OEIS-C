/*
 * A214834  a(n) is the smallest prime number such that both a(n) and (4n + 2)
 *          - a(n) are prime numbers of the form 4k + 3.
 * https://oeis.org/A214834
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the smallest prime p such that p and (4n − 2) − p are
 *            primes congruent to 3 (mod 4)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility functions from the common library */
#include "../utils/isPrimeGeneric.h"
#include "../utils/build_primes_upto.h"

/*
 * Compute a(n).  The function works for any integer type seq_t and
 * idx_t supplied by the build system.  It keeps a static list of
 * primes that covers the largest (4n−2) seen so far; this list is
 * rebuilt lazily when needed.
 */
seq_t a(idx_t n)
{
    /* Undefined for indices below the offset (n < 2) */
    if (n < 2)
        return 0;

    /* Target sum S = 4n − 2.  Compute it as idx_t then cast to seq_t. */
    idx_t S_idx = (idx_t)4 * n - 2;
    seq_t S = (seq_t)S_idx;

    /* Static cache of primes up to the current limit. */
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    static idx_t prime_limit = 0;

    /* Rebuild the prime list if the requested sum is larger than the
       current limit.  build_primes_upto allocates a new array and
       assigns it to primes; we free any previous array. */
    if (S_idx > prime_limit) {
        if (primes)
            free(primes);
        build_primes_upto(S_idx, &primes, &prime_count);
        prime_limit = S_idx;
    }

    /* Search for the smallest qualifying prime p. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];

        /* p must be of the form 4k + 3. */
        if ((p & 3ULL) != 3ULL)
            continue;

        /* If p exceeds the target sum, stop the search. */
        if ((seq_t)p > S)
            break;

        seq_t q = S - (seq_t)p;  /* Complementary prime candidate. */

        /* q must be prime and of the form 4k + 3. */
        if (((q & 3ULL) == 3ULL) && isPrimeGeneric(q))
            return (seq_t)p;
    }

    /* No qualifying pair found: return 0 as the convention for missing entries. */
    return 0;
}
