/*
 * A193744  Partial sum of Perrin numbers.
 * https://oeis.org/A193744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-4) for n > 3 (recurrence from the
 *            OEIS entry) a(0) = 3, a(1) = 3, a(2) = 5, a(3) = 8
 * Range:     u8 n <= 14, u16 n <= 34, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of already computed terms */
static seq_t *cache = NULL;
static idx_t cache_capacity = 0;   /* number of allocated slots           */
static idx_t last_computed = 0;    /* distance to last computed index     */

/* helper: initialise the cache with the first four values */
static void init_cache(void)
{
    cache_capacity = 4;
    cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    cache[0] = (seq_t)3;
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)5;
    cache[3] = (seq_t)8;
    last_computed = 3;
}

/* helper: grow the cache if needed and compute missing terms up to n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        init_cache();
    }

    if (n <= last_computed) {
        return; /* already computed */
    }

    /* grow cache if n is outside current capacity */
    if (n >= cache_capacity) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache = tmp;
        cache_capacity = n + 1;
    }

    /* compute terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* i >= 4 here because last_computed is at least 3 after init */
        seq_t val = cache[i - 1] + cache[i - 2];
        val = val - cache[i - 4];
        cache[i] = val;
    }
    last_computed = n;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
