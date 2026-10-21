/*
 * A201365  Expansion of e.g.f. exp(x) / (5 - 4*exp(x)).
 * https://oeis.org/A201365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 4 * sum_{k=0}^{n-1} binom(n,k) * a(k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

#ifndef __cplusplus
/* --------------------------------------------------------------------- */
/* C implementation: dynamic array cache                               */
/* --------------------------------------------------------------------- */
static seq_t *cache_ptr = NULL;
static size_t cache_len  = 0;

/* Resize the cache to at least `needed` entries (needed > cache_len).   */
static void grow_cache(size_t needed)
{
    if (needed <= cache_len) return;

    seq_t *tmp = (seq_t *)realloc(cache_ptr, needed * sizeof(seq_t));
    if (!tmp) abort();
    cache_ptr = tmp;

    for (size_t i = cache_len; i < needed; ++i) {
        seq_t sum = 0;
        seq_t bin = 1;                 /* binom(i,0) */
        for (size_t k = 0; k < i; ++k) {
            sum += bin * cache_ptr[k];
            if (k + 1 < i)
                bin = bin * (seq_t)(i - k) / (seq_t)(k + 1);
        }
        cache_ptr[i] = 1 + (seq_t)4 * sum;
    }
    cache_len = needed;
}
#else
/* --------------------------------------------------------------------- */
/* C++ implementation: std::vector cache                               */
/* --------------------------------------------------------------------- */
static std::vector<seq_t> cache_vec;
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    size_t need = (size_t)n + 1;
    if (cache_vec.empty())
        cache_vec.push_back(1);                     /* a(0) = 1 */
    if (cache_vec.size() < need) {
        size_t old = cache_vec.size();
        cache_vec.resize(need);
        for (size_t i = old; i < need; ++i) {
            seq_t sum = 0;
            seq_t bin = 1;
            for (size_t k = 0; k < i; ++k) {
                sum += bin * cache_vec[k];
                if (k + 1 < i)
                    bin = bin * (seq_t)(i - k) / (seq_t)(k + 1);
            }
            cache_vec[i] = 1 + (seq_t)4 * sum;
        }
    }
    return cache_vec[(size_t)n];
#else
    /* C path */
    if (cache_len == 0) {              /* initialise a(0) */
        cache_len = 1;
        cache_ptr = (seq_t *)calloc(1, sizeof(seq_t));
        if (!cache_ptr) abort();
        cache_ptr[0] = 1;
    }
    grow_cache((size_t)n + 1);
    return cache_ptr[(size_t)n];
#endif
}
