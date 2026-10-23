/*
 * A142575  Primes congruent to 45 mod 53.
 * https://oeis.org/A142575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p with p mod 53 = 45
 * Range:     u8 n <= 1, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   prime_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation searches for primes of the form 53*k+45. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/prime_candidate.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* sequence is defined for n ≥ 1 */

    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of terms stored */
    static size_t cache_cap = 0;          /* allocated capacity */

    /* next_k corresponds to the coefficient in 53*k+45, start at k=2. */
    static idx_t next_k = 2;

    /* Grow cache until it holds at least n terms. */
    while (cache_len < (size_t)n) {
        /* Resize if necessary. */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof *cache);
            cache = tmp;
        }

        /* Candidate of the form 53*k + 45. */
        idx_t cand = 53 * next_k + 45;

        /* Store the term if it is prime. */
        if (prime_candidate(cand)) {
            cache[cache_len++] = (seq_t)cand;
        }
        ++next_k;
    }

    return cache[n - 1];
}
