/*
 * A166544  a(n) = 7*n - a(n-1), with n>1, a(1)=2.
 * https://oeis.org/A166544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n - a(n-1), with a(1) = 2, for n ≥ 1
 * Range:     u8 n <= 71, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache to allow out‑of‑order calls */
static seq_t *cache = NULL;   /* array of terms, index 0 unused */
static size_t cache_cap = 0;  /* number of allocated slots (incl. index 0) */
static size_t cache_len = 0;  /* largest index fully computed */

seq_t a(idx_t n)
{
    size_t limit = (size_t)n;          /* use size_t for array indexing */

    /* First call: initialise cache with a(1) = 2 */
    if (cache_len == 0) {
        cache_cap = 2;                 /* need indices 0 and 1 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* safety */
        cache[1] = (seq_t)2;
        cache_len = 1;
    }

    /* Grow storage if necessary */
    if (limit + 1 > cache_cap) {
        size_t new_cap = limit + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* safety */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    if (limit > cache_len) {
        for (size_t i = cache_len + 1; i <= limit; ++i) {
            cache[i] = (seq_t)7 * (seq_t)i - cache[i - 1];
        }
        cache_len = limit;
    }

    return cache[limit];
}
