/*
 * A039051  Numbers whose base-8 representation has the same number of 0's and
 *          2's.
 * https://oeis.org/A039051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th positive integer whose base‑8 representation
 *            contains an equal number of 0's and 2's (with a(1)=1). The
 *            function searches successive integers until the requested term
 *            is found. It memoises previous results so that repeated calls
 *            are efficient
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   has_equal_zeros_and_twos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/has_equal_zeros_and_twos.h"

static seq_t *seq_cache = NULL;      /* memoised sequence values          */
static idx_t   cache_len  = 0;       /* number of stored terms            */
static idx_t   cache_cap  = 0;       /* allocated capacity                */
static seq_t   next_candidate = 1;   /* next integer to test              */

/* Ensure the cache has capacity for at least n elements. */
static void ensure_cache_capacity(idx_t n)
{
    if (n <= cache_cap) return;
    idx_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined, but safe return */

    /* Grow the cache until it contains the n-th term. */
    while (cache_len < n) {
        /* Find next qualifying number. */
        while (!has_equal_zeros_and_twos(next_candidate))
            next_candidate++;
        /* Store it. */
        ensure_cache_capacity(cache_len + 1);
        seq_cache[cache_len++] = next_candidate;
        /* Advance candidate for next iteration. */
        next_candidate++;
    }
    return seq_cache[n - 1];
}
