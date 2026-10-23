/*
 * A022111  Expansion of 1/((1-x)*(1-5x)*(1-6x)*(1-8x)).
 * https://oeis.org/A022111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 137*a(n-2) + 358*a(n-3) - 240*a(n-4); Initial
 *            values: a(0)=1, a(1)=20, a(2)=263, a(3)=2878
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[4] = {
    (seq_t)1,
    (seq_t)20,
    (seq_t)263,
    (seq_t)2878
};

seq_t a(idx_t n)
{
    /* static memoization cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated capacity */
    static size_t cached_count = 0; /* number of values currently stored */

    /* index for array access */
    size_t idx = (size_t)n;

    /* initialise cache with first four terms on first call */
    if (cache == NULL) {
        cache_size   = 4;
        cache        = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;  /* allocation failure, unlikely in tests */
        for (size_t i = 0; i < 4; ++i)
            cache[i] = init[i];
        cached_count = 4;
    }

    /* return directly if requested index is already computed */
    if (idx < cached_count)
        return cache[idx];

    /* grow cache if necessary */
    if (cache_size <= idx) {
        size_t new_size = cache_size;
        while (new_size <= idx)
            new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;  /* allocation failure */
        cache      = tmp;
        cache_size = new_size;
    }

    /* compute missing values up to idx */
    for (size_t i = cached_count; i <= idx; ++i) {
        seq_t v0 = cache[i - 4];  /* a(n-4) */
        seq_t v1 = cache[i - 3];  /* a(n-3) */
        seq_t v2 = cache[i - 2];  /* a(n-2) */
        seq_t v3 = cache[i - 1];  /* a(n-1) */

        /* recurrence: a(n) = 20*a(n-1) -137*a(n-2) +358*a(n-3) -240*a(n-4) */
        seq_t pos = v3 * (seq_t)20;        /* 20*a(n-1) */
        pos += v1 * (seq_t)358;           /* +358*a(n-3) */
        seq_t neg = v2 * (seq_t)137;      /* -137*a(n-2) */
        neg += v0 * (seq_t)240;           /* -240*a(n-4) */
        cache[i] = pos - neg;             /* unsigned arithmetic, guarantees pos ≥ neg */
    }

    cached_count = idx + 1;
    return cache[idx];
}
