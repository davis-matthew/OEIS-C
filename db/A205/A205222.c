/*
 * A205222  Number of (n+1)X5 0..1 arrays with the number of equal 2X2
 *          subblock diagonal pairs and equal antidiagonal pairs differing
 *          from each horizontal or vertical neighbor, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A205222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 19, u64 n <= 42, u128 n <= 89,
 *            big any n
 * Helpers:   calc_recurrence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=132, a(2)=340, … , a(16)=177055904.
   For n>16  a(n)=a(n-1)+16*a(n-2)-13*a(n-3)-98*a(n-4)+63*a(n-5)+280*a(n-6)
              -128*a(n-7)-352*a(n-8)+52*a(n-9)+150*a(n-10)+104*a(n-11)
              -72*a(n-12)+16*a(n-13)-8*a(n-14) */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/calc_recurrence.h"

static const seq_t init_vals[16] = {
    132, 340, 824, 2144, 5424, 14072, 35928, 92736,
    237864, 612368, 1573928, 4047688, 10411512, 26767752,
    68864896, 177055904
};

seq_t a(idx_t n)
{
    if (n == 0)   /* per the OEIS offset, a(0) is undefined; return 0 */
        return 0;

    /* 1‑based cache: cache[1] stores a(1). */
    static seq_t *cache = NULL;
    static idx_t cache_last = 0;   /* highest index already computed */
    static size_t cache_size = 0;  /* allocated capacity (indexable up to cache_size-1) */

    if (n <= cache_last)          /* value already computed */
        return cache[n];

    /* Ensure the cache is large enough. */
    if (!cache || cache_size <= n) {
        size_t new_size = n + 1;           /* +1 for 1‑based indexing */
        if (cache_size == 0)
            new_size = n + 1;               /* first allocation */
        else if (new_size < cache_size * 2)
            new_size = cache_size * 2;     /* growth factor 2 */
        seq_t *rc = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!rc) abort();                  /* allocation failure */
        cache = rc;
        cache_size = new_size;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 16)
            cache[i] = init_vals[i-1];
        else
            cache[i] = calc_recurrence(i, cache);
    }
    cache_last = n;
    return cache[n];
}
