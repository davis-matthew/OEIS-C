/*
 * A163715  Number of n X 3 binary arrays with all 1s connected, a path of 1s
 *          from top row to bottom row, and no 1 having more than two 1s
 *          adjacent.
 * https://oeis.org/A163715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 57, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A163715 – Number of n × 3 binary arrays with all 1s connected,
 * a path of 1s from top to bottom, and no 1 having more than two
 * adjacent 1s.
 *
 * Formula (empirical, holds for n ≥ 10):
 *   a(n) = 4*a(n-1) - 5*a(n-2) + 2*a(n-3) + a(n-4) - 2*a(n-5) + a(n-6)
 * Initial values:
 *   a(1)=6, a(2)=21, a(3)=48, a(4)=108, a(5)=236,
 *   a(6)=506, a(7)=1080, a(8)=2294, a(9)=4854
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* memoisation cache – 1‑based indexing, index 0 unused */
static seq_t *cache      = NULL;   /* cache array */
static idx_t  cache_len  = 0;      /* highest computed index */
static size_t cache_cap  = 0;      /* size of allocated array */

/* Ensure cache holds values up to index n. */
static void
ensure_cache(idx_t n)
{
    /* Allocate and initialise the cache on first use. */
    if (cache_len == 0) {
        cache_cap = 32;                 /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();

        /* store the first nine terms (index 0 unused). */
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)21;
        cache[3] = (seq_t)48;
        cache[4] = (seq_t)108;
        cache[5] = (seq_t)236;
        cache[6] = (seq_t)506;
        cache[7] = (seq_t)1080;
        cache[8] = (seq_t)2294;
        cache[9] = (seq_t)4854;
        cache_len = 9;
    }

    /* Grow the allocation if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;

        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        memcpy(new_arr, cache, (cache_len + 1) * sizeof(seq_t));
        free(cache);
        cache         = new_arr;
        cache_cap     = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* The recurrence is valid for i ≥ 10.  Because cache_len
         * is initialised to 9, the loop is entered only when i ≥ 10. */
        seq_t val = cache[i-1] * (seq_t)4
                  - cache[i-2] * (seq_t)5
                  + cache[i-3] * (seq_t)2
                  + cache[i-4]
                  - cache[i-5] * (seq_t)2
                  + cache[i-6];
        cache[i] = val;
    }
    if (n > cache_len) cache_len = n;
}

seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, but safe */
    ensure_cache(n);
    return cache[n];
}
