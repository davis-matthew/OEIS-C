/*
 * A226516  Number of (18,7)-reverse multiples with n digits.
 * https://oeis.org/A226516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 93, u32 n <= 171, u64 n <= 329,
 *            u128 n <= 645, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + a(n-10) for n > 12,
   initial values a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0,
   a(6)=1, a(7)=0, a(8)=0, a(9)=0, a(10)=0, a(11)=1, a(12)=1 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* static state shared by all calls to a() */
#ifdef __cplusplus
    static std::vector<seq_t> cacheVec;
#else
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
#endif
    static idx_t computed_up_to = 0;

#ifdef __cplusplus
    /* No cache means first call: initialise the vector up to index 12 */
    if (computed_up_to == 0 && cacheVec.empty())
    {
        cacheVec.resize(13);          /* indices 0 .. 12 inclusive   */
        cacheVec[6] = 1;
        cacheVec[11] = 1;
        cacheVec[12] = 1;
        computed_up_to = 12;
    }
#else
    size_t required = (size_t)n + 1;
    if (cache == NULL)
    {
        /* Allocate at least for indices 0 .. 12 */
        cache_len = required > 13 ? required : 13;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        if (cache == NULL) abort();
        /* Cache already zero‑initialised by calloc            */
        cache[6] = 1;
        cache[11] = 1;
        cache[12] = 1;
        computed_up_to = 12;
    }
    else if (required > cache_len)
    {
        size_t new_len = cache_len * 2;
        if (new_len < required) new_len = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) abort();
        /* Zero the newly allocated part */
        memset(tmp + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        cache = tmp;
        cache_len = new_len;
    }
#endif

    /* If the term has already been computed, return it immediately */
    if (n <= (idx_t)computed_up_to)
    {
#ifdef __cplusplus
        return cacheVec[(size_t)n];
#else
        return cache[(size_t)n];
#endif
    }

#ifdef __cplusplus
    /* Ensure the vector has space for indices up to n */
    if (cacheVec.size() <= (size_t)n) cacheVec.resize(n + 1);
#else
    /* In C, all required indices are already allocated */
#endif

    /* Compute missing terms from (computed_up_to + 1) up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i)
    {
        seq_t val = 0;
        if (i >= 2) val +=
#ifdef __cplusplus
                   cacheVec[(size_t)(i - 2)];
#else
                   cache[(size_t)(i - 2)];
#endif
        if (i >= 10) val +=
#ifdef __cplusplus
                   cacheVec[(size_t)(i - 10)];
#else
                   cache[(size_t)(i - 10)];
#endif
#ifdef __cplusplus
        cacheVec[(size_t)i] = val;
#else
        cache[(size_t)i] = val;
#endif
    }

    computed_up_to = n;
#ifdef __cplusplus
    return cacheVec[(size_t)n];
#else
    return cache[(size_t)n];
#endif
}
