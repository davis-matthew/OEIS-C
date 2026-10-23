/*
 * A142301  Primes congruent to 21 mod 44.
 * https://oeis.org/A142301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 333, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A142301 – primes congruent to 21 modulo 44.
 * The function slowly generates primes of the form 44*k+21,
 * storing them in a static cache.  Each call returns the n‑th such
 * prime, in the order used by OEIS (a(1) = 109, a(2) = 197, …).
 *
 * Algorithm: iterate k=0,1,2,…, test candidate = 44*k+21 for primality
 * using the provided is_prime() helper.  When a prime is found it is
 * appended to a dynamically growing array.  The array is reused across
 * calls, so repeated independent requests for arbitrary values of n
 * are answered quickly.
 *
 * Complexity: O(n·M(√p)) where M is the time for a single primality
 * test, which is efficient enough for n up to about 10 000.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Cached list of primes 21 mod 44 */
static seq_t *cache = NULL;   /* array of found primes */
static size_t cache_size = 0; /* number of primes stored */
static size_t cache_cap = 0;  /* allocated capacity */

/* next value of k to test (candidate = 44*k + 21) */
static idx_t next_k = 0;

seq_t a(idx_t n)
{
    /* Initialise the cache on first use */
    if (!cache) {
        cache_cap   = 16;
        cache_size  = 0;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure signal */
    }

    /* Grow cache until we have at least n entries */
    while (cache_size < (size_t)n) {
        idx_t candidate = 44 * next_k + 21;
        if (is_prime(candidate)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;      /* allocation failure signal */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)candidate;
        }
        ++next_k;
    }

    /* Return the (n-1)-th stored value (a(1) is at index 0) */
    return cache[n - 1];
}
