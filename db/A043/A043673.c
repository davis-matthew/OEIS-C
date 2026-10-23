/*
 * A043673  Numbers whose base-15 representation has exactly 6 runs.
 * https://oeis.org/A043673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth number whose base‑15 representation has exactly
 *            6 runs
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   runs_base15
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Portable implementation for seq_t and idx_t as defined by the build system.  */
#include <stdlib.h>
#include <string.h>
#include "../utils/runs_base15.h"

/* Static cache state */
static seq_t *cache = NULL;      /* array of cached terms */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static size_t cache_cnt = 0;     /* number of terms cached so far */
static seq_t curr = (seq_t)1;    /* next integer to test */

/* Grow the static cache to hold at least `needed` elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    if (cache) {
        memcpy(tmp, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of A043673 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* defined only for n ≥ 1 */

    while (cache_cnt < (size_t)n) {
        if (runs_base15(curr) == 6) {
            if (cache_cnt == cache_cap) ensure_cache_cap(cache_cnt + 1);
            cache[cache_cnt++] = curr;
        }
        curr++;
    }
    return cache[(size_t)n - 1];
}
