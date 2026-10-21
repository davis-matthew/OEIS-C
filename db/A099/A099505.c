/*
 * A099505  A transform of the Fibonacci numbers.
 * https://oeis.org/A099505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + a(n-2) - 2*a(n-3) + a(n-4) - a(n-6), initial
 *            values: a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=1, a(5)=2, a(6)=0
 * Range:     i8 n <= 25, i16 n <= 50, i32 n <= 100, i64 n <= 199,
 *            i128 n <= 395, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t base[7] = {0, 1, 1, 2, 1, 2, 0};

static seq_t *cache       = NULL; /* array of computed values */
static size_t cache_cap   = 0;     /* allocated capacity of the cache   */
static size_t cache_len   = 0;     /* number of elements actually stored */

/* ensure that cache contains values up to (and including) n */
static void ensure_up_to(idx_t n)
{
    /* initialise cache on first call */
    if (!cache) {
        cache_cap = (size_t)( (n < 7) ? 7 : n + 1 );
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* copy base values */
        for (idx_t i = 0; i < 7; ++i) cache[i] = base[i];
        cache_len = 7;
    }

    /* grow capacity if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
        cache   = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence uses indices i-1, i-2, i-3, i-4, i-6 (i>=6) */
        seq_t val = cache[i-1] + cache[i-2]
                    - 2 * cache[i-3] + cache[i-4]
                    - cache[i-6];
        cache[i] = val;
    }
    cache_len = n + 1;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    if (n < 7) return base[n];
    ensure_up_to(n);
    return cache[n];
}
