/*
 * A224674  Number of (n+1) X 7 0..2 matrices with each 2 X 2 subblock
 *          idempotent.
 * https://oeis.org/A224674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5) for n >
 *            6 initial values: a(1)=189, a(2)=300, a(3)=398, a(4)=573,
 *            a(5)=793, a(6)=1126
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* static cache to store computed values */
static seq_t *cache = NULL;      /* array of a(n), indexed from 0 (unused) */
static size_t cache_cap = 0;     /* allocated size of cache */
static idx_t cached_max = 0;     /* largest n for which a(n) is stored */

/* helper: grow the cache to at least new_cap elements */
static void
grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
    /* if allocation failed, cache becomes NULL; subsequent use will crash,
       but such a failure is outside the intended usage. */
    cache_cap = new_cap;
}

/* main function */
seq_t
a(idx_t n)
{
    if (n < 1)           /* behavior below offset is irrelevant */
        return (seq_t)0;

    /* initial values for n = 1..6 */
    static const seq_t init_vals[7] = {
        (seq_t)0,    /* index 0 unused */
        (seq_t)189,
        (seq_t)300,
        (seq_t)398,
        (seq_t)573,
        (seq_t)793,
        (seq_t)1126
    };

    /* ensure cache is initialized */
    if (cached_max == 0) {
        /* need space for indices up to 6 */
        grow_cache(7);                 /* at least 7 elements (0..6) */
        cache[0] = (seq_t)0;           /* unused */
        for (idx_t i = 1; i <= 6; ++i)
            cache[i] = init_vals[i];
        cached_max = 6;
    }

    /* if already computed, return it */
    if (n <= cached_max)
        return cache[n];

    /* grow cache to hold up to n */
    grow_cache((size_t)n + 1);

    /* compute missing values using the recurrence */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        seq_t val;
        if (k <= 6) {
            /* shouldn't happen, but guard */
            val = init_vals[k];
        } else {
            val = 4 * cache[k - 1];          /* 4*a(k-1) */
            val -= 5 * cache[k - 2];          /* -5*a(k-2) */
            val += cache[k - 3];              /* +a(k-3) */
            val += 2 * cache[k - 4];          /* +2*a(k-4) */
            val -= cache[k - 5];              /* -a(k-5) */
        }
        cache[k] = val;
    }

    cached_max = n;
    return cache[n];
}
