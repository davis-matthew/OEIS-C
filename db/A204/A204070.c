/*
 * A204070  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock having
 *          equal diagonal elements or equal antidiagonal elements, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A204070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 11*a(n-2) + 2*a(n-3), n ≥ 4 a(1) = 38, a(2) =
 *            329, a(3) = 2882 Offset: 1 (a(1) is the first term)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t  cached = 0;   /* number of elements stored (a(1) at index 0) */
    static int    initialized = 0;

    if (n == 0) return 0;   /* safety for non‑positive calls */

    /* initialise base values once */
    if (!initialized) {
        /* allocate capacity for at least 3 base values */
        cache = seq_alloc_new_array(4);
        cache[0] = (seq_t)38;
        cache[1] = (seq_t)329;
        cache[2] = (seq_t)2882;
        cached   = 3;
        initialized = 1;
    }

    /* grow cache and compute missing terms up to n */
    if (n > cached) {
        idx_t newcap = cached ? cached : 4;
        while (newcap < n) newcap <<= 1;
        seq_t *newcache = seq_alloc_new_array(newcap);
        for (idx_t i = 0; i < cached; ++i)
            newcache[i] = cache[i];
        seq_free_array(cache);
        cache = newcache;

        for (idx_t i = cached + 1; i <= n; ++i) {
            /* a(i) = 10*a(i-1) - 11*a(i-2) + 2*a(i-3) */
            seq_t a1 = cache[i - 2];   /* a(i-1) */
            seq_t a2 = cache[i - 3];   /* a(i-2) */
            seq_t a3 = cache[i - 4];   /* a(i-3) */
            cache[i - 1] = 10 * a1 + 2 * a3 - 11 * a2;
        }
        cached = n;
    }

    return cache[n - 1];
}
