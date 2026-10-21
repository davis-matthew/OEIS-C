/*
 * A059593  Number of degree-n permutations of order exactly 5.
 * https://oeis.org/A059593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (1 + a(n-5))*(n-1)(n-2)(n-3)(n-4), a(0) = a(1) =
 *            a(2) = a(3) = a(4) = 0 (A059593 – number of degree‑n
 *            permutations of order exactly 5)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* static storage for computed values */
#ifdef __cplusplus
static std::vector<seq_t> cache;
static size_t computed_max = 0;
#else
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t computed_max = 0;
#endif

static void ensure_capacity(idx_t n)
{
#ifdef __cplusplus
    /* make sure cache has entries up to n */
    if (cache.size() <= (size_t)n)
    {
        cache.resize(n + 1);
        /* initialise newly allocated elements to 0 */
        for (size_t i = computed_max + 1; i <= (size_t)n; ++i)
            cache[i] = (seq_t)0;
    }
#else
    /* C implementation – dynamic array with realloc */
    if (cache_len <= (size_t)n)
    {
        size_t new_len = cache_len == 0 ? 8 : cache_len * 2;
        if (new_len <= (size_t)n) new_len = (size_t)n + 1;
        /* cast void* to seq_t* explicitly to avoid C++ conversion error */
        void *tmp = realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) { /* allocation failed – keep cache unchanged */
            return;
        }
        cache = (seq_t *)tmp;
        /* zero initialise the new part */
        for (size_t i = cache_len; i < new_len; ++i) cache[i] = (seq_t)0;
        cache_len = new_len;
    }
    /* ensure elements from computed_max+1 up to n are zero (already true) */
#endif
}

seq_t a(idx_t n)
{
    if (n < 5)
        return (seq_t)0;   /* a(0..4) = 0 */

    ensure_capacity(n);

#ifdef __cplusplus
    /* initialise base values lazily */
    if (computed_max < 4)
    {
        /* cache already resized, just set first five entries to 0 */
        for (size_t i = 0; i <= 4; ++i) cache[i] = (seq_t)0;
        computed_max = 4;
    }
#endif

    /* compute missing terms up to n */
    for (size_t i = computed_max + 1; i <= (size_t)n; ++i)
    {
        if (i < 5)       /* safety: should not happen */
            cache[i] = (seq_t)0;
        else
        {
            /* compute (i-1)(i-2)(i-3)(i-4) as seq_t to avoid overflow */
            seq_t prod = (seq_t)(i - 1);
            prod *= (seq_t)(i - 2);
            prod *= (seq_t)(i - 3);
            prod *= (seq_t)(i - 4);
            seq_t add = (cache[i - 5] + (seq_t)1) * prod;
            cache[i] = cache[i - 1] + add;
        }
    }

    computed_max = (size_t)n;
    return cache[(size_t)n];
}
