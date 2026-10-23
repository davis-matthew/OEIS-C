/*
 * A044019  Numbers k such that 2 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that k contains the
 *            digits "2" and "9" adjacent in its decimal representation, while
 *            k + 1 does NOT contain such an adjacency
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent_29_92
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple on‑demand sieve and memoises the results
   so that subsequent calls are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_29_92.h"

/* Memoised sequence */
static seq_t *cache   = NULL;   /* array of found terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */
static seq_t next_candidate = 1u;/* next integer to test */

/* Grow the cache array as needed */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2u : 4u;
    while (new_cap < need) new_cap *= 2u;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* unrecoverable allocation failure */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0u) return 0u;            /* behaviour for n < 1 is irrelevant */

    /* Grow cache until we have n terms */
    while (cache_len < (size_t)n)
    {
        /* Find next candidate satisfying the condition */
        while (1u)
        {
            seq_t cur = next_candidate;
            if (has_adjacent_29_92(cur) && !has_adjacent_29_92(cur + 1u))
                break;                 /* found it */
            ++next_candidate;
        }

        /* Store the new term */
        ensure_cache_capacity(cache_len + 1u);
        cache[cache_len++] = next_candidate++;
    }

    return cache[n - 1u];
}
