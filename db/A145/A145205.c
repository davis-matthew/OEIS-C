/*
 * A145205  Numbers X such that (X^2-67)/201 is a square.
 * https://oeis.org/A145205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1030190*a(n-1) – a(n-2) for n ≥ 3, with a(1) = 4154, a(2)
 *            = 4279405106
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache of previously computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored (a(1) at index 0) */
static size_t cache_cap = 0;   /* allocated capacity */

/* ensure the cache is large enough to contain terms up to n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)      /* already computed */
        return;

    idx_t new_len = n;
    if ((size_t)new_len > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap < (size_t)new_len)
            new_cap <<= 1;

#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
#else
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            exit(1);          /* out of memory */
#endif
        /* copy existing terms */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* release old storage */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i < new_len; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)4154;          /* a(1) */
        } else if (i == 1) {
            cache[1] = (seq_t)4279405106;    /* a(2) */
        } else {
            cache[i] = (seq_t)1030190 * cache[i-1] - cache[i-2];
        }
    }
    cache_len = new_len;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n-1];               /* a(1) at index 0 */
}
