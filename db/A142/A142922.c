/*
 * A142922  Primes congruent to 59 mod 63.
 * https://oeis.org/A142922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 175, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142922: primes congruent to 59 (mod 63)
   The implementation searches each candidate 59, 122, 185, …,
   keeping a cache of found primes so that any call to a(n)
   runs in O(n) time overall.  No hard‑coded values are used.   */

#include <stdlib.h>     /* malloc, realloc */
#include <stddef.h>     /* size_t */
#include "../utils/is_prime_small.h"   /* is_prime_small(seq_t) */

/* Return the n-th term (n ≥ 1) of sequence A142922 */
seq_t a(idx_t n)
{
    /* Cached primes */
    static seq_t *cache = NULL;   /* array of already found primes   */
    static idx_t  cache_size = 0; /* number of primes stored         */
    static idx_t  cache_cap   = 0; /* allocated capacity (in elements) */

    /* Return cached value if already known */
    if (n > 0 && n <= cache_size)
        return cache[n - 1];

    /* Ensure the cache is large enough for n elements */
    if (cache_cap < n) {
        size_t new_cap = (cache_cap == 0) ? 16 : (size_t)cache_cap * 2;
        if (new_cap < (size_t)n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)   /* allocation failed – should never happen  */
            return (seq_t)0;
        cache      = tmp;
        cache_cap  = (idx_t)new_cap;
    }

    /* Candidate number: 59 if no primes yet, otherwise last prime + 63 */
    seq_t candidate = (cache_size == 0) ? (seq_t)59
                                      : cache[cache_size - 1] + (seq_t)63;

    /* Find successive primes until the desired index is reached */
    while (cache_size < n) {
        if (is_prime_small(candidate))
            cache[cache_size++] = candidate;
        candidate += (seq_t)63;   /* next number ≡ 59 (mod 63) */
    }

    return cache[n - 1];
}
