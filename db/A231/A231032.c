/*
 * A231032  Number of n X 3 0..2 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 3 and at least one element with value
 *          (x(i,j)-1) mod 3, and upper left element zero.
 * https://oeis.org/A231032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A231032: Number of n×3 arrays over {0,1,2} with each cell adjacent
   to at least one neighbor of value (x+1) mod 3 and at least one of
   (x-1) mod 3, and with upper-left cell fixed to 0.
   Empirical recurrence (valid for n≥10):
   a(n) = 9*a(n-1) - a(n-2) + 15*a(n-3) - 374*a(n-4) + 260*a(n-5)
          + 256*a(n-6) + 60*a(n-7) - 176*a(n-8) - 32*a(n-9)
   Initial values:
     a(1)=0, a(2)=0, a(3)=10, a(4)=32, a(5)=278,
     a(6)=2988, a(7)=23058, a(8)=199272, a(9)=1722118
*/

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *cache = NULL;            /* 1‑based array of computed terms */
static idx_t  cache_cap = 0;            /* current allocated capacity          */
static idx_t  last_computed = 0;        /* highest n for which a(n) is known    */
static bool   init_done   = false;      /* whether base values are inserted    */

static void
ensure_cache(idx_t n)
{
    /* Grow the cache if necessary. */
    if (n <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 10;
    while (new_cap < n) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (!new_cache) return; /* allocation failure – return, caller will fail */

    if (cache) {
        for (idx_t i = 1; i <= cache_cap; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;

    if (!init_done) {
        /* Insert the first nine known terms.  Index 0 is unused. */
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 10;
        cache[4] = 32;
        cache[5] = 278;
        cache[6] = 2988;
        cache[7] = 23058;
        cache[8] = 199272;
        cache[9] = 1722118;
        init_done = true;
        last_computed = 9;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; return 0 for safety      */
    ensure_cache(n);
    if (n <= last_computed) return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 9) {                /* base values already set */
            /* Nothing to do */
        } else {
            /* Use the empirical recurrence */
            seq_t sum_pos = cache[i-1] * 9
                          + cache[i-3] * 15
                          + cache[i-5] * 260
                          + cache[i-6] * 256
                          + cache[i-7] * 60;
            seq_t sum_neg = cache[i-2]
                          + cache[i-4] * 374
                          + cache[i-8] * 176
                          + cache[i-9] * 32;
            cache[i] = sum_pos - sum_neg;
        }
        last_computed = i;
    }
    return cache[n];
}
