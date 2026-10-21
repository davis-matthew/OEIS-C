/*
 * A078912  a(1)=a(2)=1, a(n)=a(n-1)+a(n-2) if n is odd, a(n)=a(n-1)+a(n/2) if
 *          n is even.
 * https://oeis.org/A078912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078912:  a(1)=a(2)=1;
   a(n)=a(n-1)+a(n-2)   if n is odd;
   a(n)=a(n-1)+a(n/2)   if n is even.
   Computed iteratively with a static cache so calls can be in any order. */
#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

#ifdef __cplusplus
    static std::vector<seq_t> cacheVec(2, 0);          /* indices 0,1; 0 unused */
    size_t size_before = cacheVec.size();

    /* grow the vector if needed */
    if ((size_t)n + 1 > size_before) {
        cacheVec.resize((size_t)n + 1);
    }

    /* initialise base values if this is the first use or if they were missing */
    if (n >= 1) cacheVec[1] = (seq_t)1;
    if (n >= 2) cacheVec[2] = (seq_t)1;

    /* compute new terms only if necessary */
    size_t start = size_before > 3 ? size_before : 3;
    for (size_t i = start; i <= (size_t)n; ++i) {
        if (i & 1)                     /* odd */
            cacheVec[i] = cacheVec[i - 1] + cacheVec[i - 2];
        else                           /* even */
            cacheVec[i] = cacheVec[i - 1] + cacheVec[i / 2];
    }
    return cacheVec[(size_t)n];
#else
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    size_t size_before = cache_len;

    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failed */
        cache = tmp;
        cache_len = new_len;
        /* initialise base values */
        if (n >= 1) cache[1] = (seq_t)1;
        if (n >= 2) cache[2] = (seq_t)1;
    }

    /* compute new terms only if necessary */
    size_t start = size_before > 3 ? size_before : 3;
    for (size_t i = start; i <= (size_t)n; ++i) {
        if (i & 1)                     /* odd */
            cache[i] = cache[i - 1] + cache[i - 2];
        else                           /* even */
            cache[i] = cache[i - 1] + cache[i / 2];
    }
    return cache[(size_t)n];
#endif
}
