/*
 * A029360  Expansion of 1/((1-x^4)*(1-x^7)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A029360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 4, 7, 8, 10
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Static cache for previously computed values. */
static idx_t cache_len = 0;            /* number of valid elements */

#ifdef __cplusplus
/* Vector form – used when compiling with C++ */
static std::vector<seq_t> cache_vec;

/* Recompute cache for indices 0 … N when required. */
static void ensure_cache(idx_t N)
{
    cache_vec.resize(N + 1);
    cache_len = N + 1;

    /* Set all entries to 0, except cache_vec[0] = 1. */
    for (idx_t i = 0; i < cache_len; ++i)
        cache_vec[i] = (seq_t)0;
    cache_vec[0] = (seq_t)1;

    static const idx_t parts[4] = {4, 7, 8, 10};
    for (int pi = 0; pi < 4; ++pi) {
        idx_t part = parts[pi];
        for (idx_t i = part; i <= N; ++i)
            cache_vec[i] = cache_vec[i] + cache_vec[i - part];
    }
}
#else
/* Raw pointer form – used for C compiles. */
static seq_t *cache = NULL;

static void ensure_cache(idx_t N)
{
    cache_len = N + 1;
    cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    if (!cache) abort();

    /* Set all entries to 0, except cache[0] = 1. */
    for (idx_t i = 0; i < cache_len; ++i)
        cache[i] = (seq_t)0;
    cache[0] = (seq_t)1;

    static const idx_t parts[4] = {4, 7, 8, 10};
    for (int pi = 0; pi < 4; ++pi) {
        idx_t part = parts[pi];
        for (idx_t i = part; i <= N; ++i)
            cache[i] = cache[i] + cache[i - part];
    }
}
#endif

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    if (n >= cache_len)
        ensure_cache(n);
    return cache_vec[n];
#else
    if (n >= cache_len)
        ensure_cache(n);
    return cache[n];
#endif
}
