/*
 * A142718  Odd terms in A014217.
 * https://oeis.org/A142718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 19*a(n-3) - 19*a(n-6) + a(n-9), offset 1 initial terms:
 *            a(1)=1, a(2)=1, a(3)=11, a(4)=17, a(5)=29, a(6)=199, a(7)=321,
 *            a(8)=521, a(9)=3571
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* 1‑indexed cache, cache[0] unused */
static idx_t  cache_max  = 0;      /* highest index computed */
static size_t cache_size = 0;      /* allocated capacity */

/* Ensure the cache can hold index n (1‑based). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 > cache_size) {
        size_t new_cap = (cache_size == 0 ? (size_t)n + 1 : cache_size * 2);
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* out of memory */
        cache = tmp;
        cache_size = new_cap;
    }
}

static const seq_t init[9] = {
    (seq_t)1,  (seq_t)1,  (seq_t)11,  (seq_t)17,  (seq_t)29,
    (seq_t)199, (seq_t)321, (seq_t)521, (seq_t)3571
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset 1 – a(0) unused */

    ensure_cache(n);                       /* expand cache if needed */

    /* Store base values if not yet stored */
    if (cache_max < 9) {
        idx_t limit = (n < 9) ? n : 9;
        for (idx_t i = 1; i <= limit; ++i)
            cache[i] = init[i - 1];
        cache_max = limit;
    }

    /* Compute terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init[i - 1];        /* safety guard */
        } else {
            cache[i] = 19 * cache[i - 3]
                      - 19 * cache[i - 6]
                      +  cache[i - 9];
        }
    }
    cache_max = n;
    return cache[n];
}
