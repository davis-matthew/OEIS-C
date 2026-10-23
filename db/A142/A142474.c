/*
 * A142474  1 followed by A141015.
 * https://oeis.org/A142474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(1)=1, a(2)=0, a(3)=1 a(n) = a(n-1) + 2*a(n-2) + a(n-3) for n>3
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 61, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values, 1-indexed */
static idx_t cache_size = 0;         /* highest computed index */
static idx_t cache_cap   = 0;         /* allocated size (indexable up to cache_cap-1) */

/* initialise cache with first three terms */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_cap = 4; /* indices 0..3 are used, 0 is unused */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache_size = 3;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe fallback */

    init_cache();

    if (n <= 3) return cache[n];

    if (n > cache_size) {
        /* enlarge allocation if necessary */
        idx_t new_cap = n + 1;
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing values */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = cache[i-1] + (seq_t)2 * cache[i-2] + cache[i-3];
        }
        cache_size = n;
    }

    return cache[n];
}
