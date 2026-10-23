/*
 * A142599  Second trisection of A061037.
 * https://oeis.org/A142599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 86, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142599 – Second trisection of A061037
   Formula: a(n) = 3*a(n-4) - 3*a(n-8) + a(n-12)   (for n > 12)
   Initial values: a(1)=5, a(2)=2, a(3)=77, a(4)=35, a(5)=221,
                    a(6)=20, a(7)=437, a(8)=143, a(9)=725,
                    a(10)=56, a(11)=1085, a(12)=323
*/

#include <stdlib.h>

/* initial terms for indices 1..12 */
static const seq_t init[13] = {
    0,   /* dummy for 0 */
    5,   /* a(1) */
    2,   /* a(2) */
    77,  /* a(3) */
    35,  /* a(4) */
    221, /* a(5) */
    20,  /* a(6) */
    437, /* a(7) */
    143, /* a(8) */
    725, /* a(9) */
    56,  /* a(10) */
    1085,/* a(11) */
    323  /* a(12) */
};

/* cache for computed terms, indices start at 1 */
static seq_t *cache = NULL;
static idx_t cache_size = 0; /* number of valid entries in cache (>=0) */

static void ensure_cache(idx_t n)
{
    /* cache must hold indices 1 .. n inclusive */
    if (cache_size >= n + 1)
        return;                 /* already sufficient */

    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        abort();                 /* memory allocation failed */
    }
    cache = tmp;

    /* set initial values if not already present */
    for (idx_t i = 1; i <= 12 && i <= new_size; ++i) {
        if (i >= cache_size) {
            cache[i] = init[i];
        }
    }

    /* compute recurrence for remaining indices */
    idx_t start = (cache_size > 12) ? cache_size : 13;
    for (idx_t i = start; i < new_size; ++i) {
        cache[i] = 3 * cache[i-4] - 3 * cache[i-8] + cache[i-12];
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* sequence is 1‑based; undefined for n<1 */
    if (n <= 12)
        return init[n];

    ensure_cache(n);
    return cache[n];
}
