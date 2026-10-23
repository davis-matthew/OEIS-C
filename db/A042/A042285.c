/*
 * A042285  Denominators of continued fraction convergents to sqrt(668).
 * https://oeis.org/A042285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 112894*a(n-8) - a(n-16)
 * Initial values:
 *   a(0)=1, a(1)=1, a(2)=6, a(3)=13, a(4)=162, a(5)=337,
 *   a(6)=1847, a(7)=2184, a(8)=111047, a(9)=113231,
 *   a(10)=677202, a(11)=1467635, a(12)=18288822,
 *   a(13)=38045279, a(14)=208515217, a(15)=246560496
 */

#include <stdlib.h> /* malloc, realloc, abort */

static const seq_t base_vals[16] = {
    (seq_t)1, (seq_t)1, (seq_t)6, (seq_t)13,
    (seq_t)162, (seq_t)337, (seq_t)1847, (seq_t)2184,
    (seq_t)111047, (seq_t)113231, (seq_t)677202, (seq_t)1467635,
    (seq_t)18288822, (seq_t)38045279, (seq_t)208515217, (seq_t)246560496
};

static seq_t *cache = (seq_t*)NULL;
static idx_t cache_size = 0;     /* number of allocated elements          */
static idx_t last_computed = 0;  /* highest index computed (>= 15 after init) */

seq_t a(idx_t n)
{
    /* Lazy initialization: ensure base values are cached */
    if (cache == (seq_t*)NULL) {
        cache_size = 16;
        cache = (seq_t*)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i < 16; ++i)
            cache[i] = base_vals[i];
        last_computed = 15;
    }

    /* Grow the cache to hold up to index n if necessary */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = (idx_t)new_size;
    }

    /* Compute all missing terms up to n using the recurrence */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t x1 = cache[i - 8];
            seq_t x2 = cache[i - 16];
            cache[i] = (seq_t)112894 * x1 - x2;
        }
        last_computed = n;
    }

    return cache[n];
}
