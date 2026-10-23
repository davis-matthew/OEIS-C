/*
 * A047056  Quintuple factorial numbers: Product_{k=0..n-1} (5*k+3).
 * https://oeis.org/A047056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A047056: Quintuple factorial numbers
 * Formula: a(n) = ∏_{k=0}^{n-1} (5*k+3)
 * Recurrence: a(0)=1, a(n) = (5*n-2)*a(n-1) for n>=1
 */

#ifndef __cplusplus
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation: use std::vector to handle non‑POD seq_t */
    #include <vector>
    static std::vector<seq_t> cache_vec;
    idx_t old_size = cache_vec.size();

    if (old_size == 0) {
        cache_vec.resize(1);
        cache_vec[0] = (seq_t)1;
        old_size = 1;
    }

    if (n + 1 > cache_vec.size())
        cache_vec.resize(n + 1);

    for (idx_t i = old_size; i <= n; ++i) {
        seq_t factor = (seq_t)(5 * i - 2);
        cache_vec[i] = cache_vec[i - 1] * factor;
    }

    return cache_vec[n];
#else
    /* C implementation: use realloc on a raw array of seq_t */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* grow capacity if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) abort();   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise a(0) if needed */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t factor = (seq_t)(5 * i - 2);
        cache[i] = cache[i - 1] * factor;
    }
    cache_len = n + 1;

    return cache[n];
#endif
}
