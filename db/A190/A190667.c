/*
 * A190667  Expansion of (1+2*x)/(1-x^4-2*x^3-2*x^2-x).
 * https://oeis.org/A190667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+2*a(n-2)+2*a(n-3)+a(n-4) with a(0)=1, a(1)=3,
 *            a(2)=5, a(3)=13
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
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
#ifndef __cplusplus
    /* C implementation ----------------------------------------------------- */
    /* static cache of values */
    static seq_t *cache = NULL;
    static idx_t size = 0;

    /* initialise base values if first call */
    if (size == 0) {
        size = 4;
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (!cache) abort();          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)13;
    }

    /* extend cache if needed */
    if (n + 1 > size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();      /* allocation failed */

        /* compute new entries */
        for (idx_t i = size; i < new_size; ++i) {
            seq_t v1 = new_cache[i-1];
            seq_t v2 = new_cache[i-2];
            seq_t v3 = new_cache[i-3];
            seq_t v4 = new_cache[i-4];
            new_cache[i] = v1 + (v2 << 1) + (v3 << 1) + v4;
        }
        cache = new_cache;
        size = new_size;
    }

    return cache[n];
#else
    /* C++ implementation ---------------------------------------------------- */
    /* static vector cache */
    static std::vector<seq_t> vec;
    if (vec.empty()) {
        vec.reserve(4);
        vec.push_back((seq_t)1);
        vec.push_back((seq_t)3);
        vec.push_back((seq_t)5);
        vec.push_back((seq_t)13);
    }

    while (vec.size() <= n) {
        idx_t i = vec.size();           /* index of new element */
        seq_t v1 = vec[i-1];
        seq_t v2 = vec[i-2];
        seq_t v3 = vec[i-3];
        seq_t v4 = vec[i-4];
        vec.push_back(v1 + (v2 << 1) + (v3 << 1) + v4);
    }

    return vec[n];
#endif
}
