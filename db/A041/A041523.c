/*
 * A041523  Denominators of continued fraction convergents to sqrt(278).
 * https://oeis.org/A041523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 5002*a(n-6) - a(n-12) for n>11; Initial values: a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=49, a(4)=101, a(5)=150, a(6)=4901,
 *            a(7)=5051, a(8)=15003, a(9)=245099, a(10)=505201, a(11)=750300
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values 0 .. cache_size-1   */
static idx_t  cache_size = 0;        /* number of valid entries          */
static idx_t  cache_cap  = 0;        /* allocated capacity (in seq_t elems) */

static const seq_t init[12] = {
    1, 1, 3, 49, 101, 150,
    4901, 5051, 15003, 245099, 505201, 750300
};

static void ensure_cache(idx_t n)
{
    /* grow the cache so that indices 0..n are valid */
    if (cache_cap < n + 1) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – abort program (discarded in OEIS test harness) */
            abort();
        }
        cache = new_cache;
        cache_cap = (idx_t)new_cap;
    }

    /* initialise first 12 terms if cache is new */
    if (cache_size == 0) {
        for (idx_t i = 0; i < 12; ++i) cache[i] = init[i];
        cache_size = 12;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = ((seq_t)5002) * cache[i - 6] - cache[i - 12];
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 12) return init[n];
    ensure_cache(n);
    return cache[n];
}
