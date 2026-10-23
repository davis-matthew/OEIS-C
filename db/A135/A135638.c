/*
 * A135638  Values of z in positive solutions to the Diophantine equation
 *          43x+7y+17z=400.
 * https://oeis.org/A135638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   enumerate all positive integer solutions of 43x+7y+17z=400,
 *            order them by increasing x and for equal x by increasing y, and
 *            output the z‑coordinate of each solution in that order
 * Range:     u8 none, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Grow a dynamic array. */
static void grow_cache(size_t needed, size_t *cap, void **ptr)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap) ? (*cap) << 1 : 8;
    if (newcap < needed) newcap = needed;
    seq_t *tmp = (seq_t *)realloc(*ptr, newcap * sizeof(seq_t));
    if (!tmp) abort();        /* malloc failure */
    *ptr = tmp;
    *cap = newcap;
}

/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* sequence values       */
    static size_t cache_cap  = 0;      /* capacity of cache     */
    static size_t cache_len  = 0;      /* current length        */

    /* Build the cache only once. */
    if (cache_len == 0) {
        /* Enumerate x, y, compute z = (400-43x-7y)/17. */
        for (idx_t x = 1; ; ++x) {
            seq_t sum_x = (seq_t)43 * (seq_t)x;
            if (sum_x > 400 - 7 - 17) break; /* no larger x possible */

            for (idx_t y = 1; ; ++y) {
                seq_t sum_xy = sum_x + (seq_t)7 * (seq_t)y;
                if (sum_xy > 400 - 17) break; /* no larger y possible */

                seq_t rem = 400 - sum_xy;
                if (rem % 17 == 0) {
                    seq_t z = rem / 17;

                    /* Ensure space for the new value. */
                    grow_cache(cache_len + 1, &cache_cap, (void **)&cache);
                    cache[cache_len++] = z;
                }
            }
        }
    }

    /* Indexing: a(1) is first element. */
    if (n < 1) return (seq_t)0;
    idx_t idx = n - 1;
    if (idx >= cache_len) return (seq_t)0;
    return cache[idx];
}
