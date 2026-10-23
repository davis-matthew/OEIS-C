/*
 * A133631  a(n) = a(n-1) - 4*a(n-2), a(0)=1, a(1)=2.
 * https://oeis.org/A133631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1, a(1) = 2, a(n) = a(n-1) - 4*a(n-2) for n ≥ 2
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 30, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, free */

/* static cache to allow out‑of‑order calls */
static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_size = 0;     /* number of computed terms */

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* ensure cache is large enough */
    size_t idx = (size_t)n;
    if (cache_cap < idx + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= idx) new_cap *= 2;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failure fallback */
        for (size_t i = 0; i < cache_cap; ++i) tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialize base values if not already done */
    if (cache_size < 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_size = 2;
    }

    /* compute missing terms up to idx */
    for (size_t i = cache_size; i <= idx; ++i) {
        cache[i] = cache[i-1] - (seq_t)4 * cache[i-2];
    }
    cache_size = idx + 1;

    return cache[idx];
}
