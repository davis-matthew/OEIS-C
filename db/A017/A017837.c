/*
 * A017837  Expansion of 1/(1 - x^5 - x^6).
 * https://oeis.org/A017837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5) + a(n-6), a(0) = 1, a(1)=a(2)=a(3)=a(4)=0, a(5) =
 *            1
 * Range:     u8 n <= 58, u16 n <= 101, u32 n <= 188, u64 n <= 364,
 *            u128 n <= 715, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ compilation path – use std::vector to get default construction of
     * arbitrary‑precision objects.  The vector is resized only when needed
     * and filled incrementally so that any order of calls works. */
    static std::vector<seq_t> cache;
    static idx_t last = 0;

    /* initialise a small cache once */
    if (cache.empty()) {
        cache.resize(16);
        cache[0] = 1;
        cache[5] = 1;
    }

    /* grow if required */
    if (n >= cache.size()) {
        cache.resize(n + 1);
        cache[0] = 1;
        cache[5] = 1;
    }

    /* compute any values that have not yet been cached */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 4)
            cache[i] = 0;
        else if (i == 5)
            cache[i] = 1;
        else
            cache[i] = cache[i - 5] + cache[i - 6];
    }
    last = n;
    return cache[n];
#else
    /* C compilation path – use malloc/realloc and zero‑initialisation. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t last = 0;

    /* initial allocation */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = 1;
        cache[5] = 1;
    }

    /* grow the cache if required */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        if (new_cap > cache_cap) {
            for (idx_t i = cache_cap; i < new_cap; ++i)
                tmp[i] = 0;
        }
        cache = tmp;
        cache_cap = new_cap;
        cache[0] = 1;
        cache[5] = 1;  /* unchanged, but keep the invariant */
    }

    /* compute any values that have not yet been cached */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 4)
            cache[i] = 0;
        else if (i == 5)
            cache[i] = 1;
        else
            cache[i] = cache[i - 5] + cache[i - 6];
    }
    last = n;
    return cache[n];
#endif
}
