/*
 * A142558  Primes congruent to 28 mod 53.
 * https://oeis.org/A142558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Primes congruent to 28 mod 53 (OEIS A142558)
 * Range:     u8 none, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* Array of found primes */
static size_t cache_count = 0;       /* Number of primes stored */
static size_t cache_cap = 0;         /* Capacity of the array */
static seq_t next_candidate = 28;    /* Next number to test (congruence 28 mod 53) */

/* Helper to ensure the cache has space for at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();             /* Out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* Undefined for offset 0 */
    /* Generate primes until we have at least n of them */
    while (cache_count < n)
    {
        /* Find next candidate that is 28 mod 53 and prime */
        seq_t cand = next_candidate;
        next_candidate += 53;
        if (cand < 2) continue;           /* Skip numbers < 2 */
        if (is_prime((idx_t)cand))
        {
            /* Store the prime (cast to seq_t to keep type validity) */
            ensure_cache_cap(cache_count + 1);
            cache[cache_count++] = cand;
        }
    }
    return cache[n - 1];
}
