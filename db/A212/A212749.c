/*
 * A212749  Number of (w,x,y,z) with all terms in {0,...,n} and at least one
 *          of these conditions holds: w=R, x<R, y<R, z<R, where R =
 *          max{w,x,y,z} - min{w,x,y,z}.
 * https://oeis.org/A212749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            2*a(n-1)+2*a(n-2)-6*a(n-3)+6*a(n-5)-2*a(n-6)-2*a(n-7)+a(n-8)
 *            Initial values: a(0)=1, a(1)=14, a(2)=70, a(3)=222, a(4)=537,
 *            a(5)=1116, a(6)=2056, a(7)=3512
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation cache */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = 0;

/* Ensure the cache has at least `needed` elements */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * needed);
    if (!new_cache)
        return; /* allocation failure – in production code abort or handle */
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = needed;
}

/* Fill the first eight values (required for the recurrence) */
static void init_first_eight(void)
{
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)14, (seq_t)70, (seq_t)222,
        (seq_t)537, (seq_t)1116, (seq_t)2056, (seq_t)3512
    };
    ensure_capacity(8);
    for (size_t i = 0; i < 8; ++i)
        cache[i] = init[i];
    last_computed = 7;
}

seq_t a(idx_t n)
{
    /* Lazy initialisation */
    if (cache == NULL)
        init_first_eight();

    /* Base cases */
    if ((size_t)n < 8)
        return cache[(size_t)n];

    size_t target = (size_t)n;

    /* Ensure enough storage */
    if (cache_cap <= target)
        ensure_capacity(target + 1);

    /* Compute missing terms */
    if (last_computed < target) {
        for (size_t k = last_computed + 1; k <= target; ++k) {
            /* Separate positive and negative parts to avoid underflow
               for unsigned types: 2*a[k-1]+2*a[k-2]+6*a[k-5]+a[k-8]
                                    – (6*a[k-3]+2*a[k-6]+2*a[k-7])    */
            seq_t positive = 2 * cache[k-1] + 2 * cache[k-2] + 6 * cache[k-5] + cache[k-8];
            seq_t negative = 6 * cache[k-3] + 2 * cache[k-6] + 2 * cache[k-7];
            cache[k] = positive - negative;
        }
        last_computed = target;
    }

    return cache[target];
}
