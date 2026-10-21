/*
 * A224859  Numbers n such that 90*n + 47 and 90*n + 49 are twin primes.
 * https://oeis.org/A224859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 81, u32 n <= 82, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224859: numbers n such that 90*n + 47 and 90*n + 49 are twin primes.
   The function searches for such n in order and caches the results. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* memoisation state */
    static idx_t         last_checked = 0;   /* smallest candidate not yet examined */
    static seq_t        *cache        = NULL; /* cached nth terms */
    static size_t        cache_len    = 0;    /* number of cached terms */
    static size_t        cache_cap    = 0;    /* allocated capacity */

    /* extend cache until we have at least n terms */
    while (cache_len < n)
    {
        /* advance to next candidate n */
        ++last_checked;
        idx_t cand_idx = last_checked;

        /* compute the two numbers of the twin‑prime pair in seq_t to avoid overflow */
        seq_t cand  = (seq_t)cand_idx;
        seq_t p1    = cand * (seq_t)90 + (seq_t)47;
        seq_t p2    = p1 + (seq_t)2;

        /* test for twin primes */
        if (is_prime_big(p1) && is_prime_big(p2))
        {
            /* grow cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failure: stop and return 0 */
                    if (cache) free(cache);
                    cache        = NULL;
                    cache_len    = 0;
                    cache_cap    = 0;
                    return 0;
                }
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand_idx;
        }
    }

    return cache[n - 1];  /* 1‑indexed sequence */
}
