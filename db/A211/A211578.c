/*
 * A211578  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having
 *          three, four, five or six distinct values for every i,j,k<=n.
 * https://oeis.org/A211578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 59,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) + 12*a(n-2) - 21*a(n-3) - 48*a(n-4)
 *              + 64*a(n-5) + 80*a(n-6) - 60*a(n-7) - 48*a(n-8)
 *   a(1) = 16, a(2) = 52, a(3) = 144, a(4) = 380, a(5) = 964,
 *   a(6) = 2456, a(7) = 6124, a(8) = 15508
 *
 * This implementation memoises the values using a dynamic cache.
 * It uses the provided helper seq_alloc_new_array to allocate arrays
 * of seq_t objects, which works for both built‑in integer types and
 * arbitrary‑precision big integer classes.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"

/* Cache of computed values a(1)..a(N) */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements actually computed */
static size_t cache_cap = 0;   /* allocated capacity */

/* Base values for n = 1..8 */
static const seq_t base_vals[8] = {
    16, 52, 144, 380, 964, 2456, 6124, 15508
};

/* Grow the cache to at least new_cap elements, copying existing data. */
static void grow_cache_to(size_t new_cap)
{
    seq_t *new_arr = seq_alloc_new_array(new_cap);
    if (!new_arr) abort();            /* out‑of‑memory */
    for (size_t i = 0; i < cache_len; ++i)
        new_arr[i] = cache[i];        /* copy existing values */
    cache = new_arr;
    cache_cap = new_cap;
    /* Old array is left to be freed by the environment; leaking a few
       bytes for n <= 10000 is acceptable. */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1 */

    size_t needed = (size_t)n;        /* number of values to compute */
    if (cache_len < needed) {
        /* Ensure enough capacity in the cache. */
        if (needed > cache_cap)
            grow_cache_to(needed >= 8 ? needed : 8);

        /* Compute missing values. */
        for (size_t i = cache_len; i < needed; ++i) {
            if (i < 8) {                      /* base values */
                cache[i] = base_vals[i];
            } else {                          /* recurrence */
                cache[i] = 2*cache[i-1] + 12*cache[i-2]
                           - 21*cache[i-3] - 48*cache[i-4]
                           + 64*cache[i-5] + 80*cache[i-6]
                           - 60*cache[i-7] - 48*cache[i-8];
            }
        }
        cache_len = needed;
    }
    return cache[n-1];                    /* cache is zero‑based */
}
