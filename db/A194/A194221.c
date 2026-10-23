/*
 * A194221  Partial sums of A194220.
 * https://oeis.org/A194221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 591, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A194221 – Partial sums of A194220
 *
 *  Formula:
 *      a(1) = 0, a(2) = 0, a(3) = 1, a(4) = 2, a(5) = 3, a(6) = 5,
 *      a(7) = 8, a(8) = 11, a(9) = 14, a(10) = 17
 *
 *      a(n) = 2*a(n-1) - a(n-2) + a(n-8) - 2*a(n-9) + a(n-10)   for n > 10
 *
 *  The function below caches values so that calls may be made in any order.
 *  It uses the utility functions `seq_alloc_array` and `seq_free_array` for
 *  memory management.  No other external headers or libraries are required.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* Base values for 1 ≤ n ≤ 10 (index 0 unused). */
static const seq_t base[11] = {
    0,      /* base[0] – unused */
    0,      /* a(1) */
    0,      /* a(2) */
    1,      /* a(3) */
    2,      /* a(4) */
    3,      /* a(5) */
    5,      /* a(6) */
    8,      /* a(7) */
    11,     /* a(8) */
    14,     /* a(9) */
    17      /* a(10) */
};

/* Cached sequence values.  cache[0] is unused; cache[i] holds a(i). */
static seq_t *cache = NULL;
static idx_t cache_size = 0;  /* highest index stored (0 if none) */

/* Ensure that cache contains all values up to n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* Allocate a new array for indices 0 … n (index 0 unused). */
    seq_t *new_cache = seq_alloc_array((size_t)n + 1);
    if (cache != NULL) {
        /* Copy existing values to the new array. */
        for (idx_t i = 1; i <= cache_size; ++i)
            new_cache[i] = cache[i];

        /* Free the old cache. */
        seq_free_array(cache);
    }

    /* Compute new entries. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 10) {
            new_cache[i] = base[i];
        } else {
            new_cache[i] =  2 * new_cache[i-1]
                            - new_cache[i-2]
                            + new_cache[i-8]
                            - 2 * new_cache[i-9]
                            + new_cache[i-10];
        }
    }

    cache      = new_cache;
    cache_size = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined for n = 0; defensive */

    if (n <= 10)
        return base[n];

    ensure_cache(n);
    return cache[n];
}
