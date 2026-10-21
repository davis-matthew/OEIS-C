/*
 * A063217  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          50 ).
 * https://oeis.org/A063217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + a(n-3) - a(n-5)
   Initial values: a(1)=2, a(2)=5, a(3)=7, a(4)=12, a(5)=14
   Offset: 1
   The array is 1‑based; a[0] is unused. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array of cached values */
static idx_t  cache_max = 0;         /* highest index currently cached */

/* Extend the cache up to at least n and compute new terms recursively. */
static void ensure_cache(idx_t n)
{
    if (cache_max >= n)
        return;                     /* already computed */

    /* Initial allocation: ensure indices 1..5 exist */
    if (cache == NULL) {
        size_t init_size = 6;               /* indices 0..5 */
        cache = (seq_t *)malloc(sizeof(seq_t) * init_size);
        if (!cache) return;                 /* allocation failure: will crash later */

        /* Initialize base cases */
        cache[0] = 0;                    /* unused */
        cache[1] = 2;
        cache[2] = 5;
        cache[3] = 7;
        cache[4] = 12;
        cache[5] = 14;
        cache_max = 5;
    }

    /* Grow the array if necessary */
    if (n > cache_max) {
        size_t new_size = cache_max * 2 + 1;   /* doubling strategy, +1 to keep 0.. */
        if (new_size < (size_t)(n + 1))
            new_size = n + 1;                 /* at least n+1 elements */

        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!new_cache) return;                 /* allocation failure: will crash later */
        cache = new_cache;

        /* Compute new terms sequentially */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            cache[i] = cache[i-2] + cache[i-3] - cache[i-5];
        }

        cache_max = n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is undefined for n = 0; return 0 as a safe default. */
        return 0;
    }
    ensure_cache(n);
    return cache[n];
}
