/*
 * A098182  a(n) = 3*a(n-1) - a(n-2) + a(n-3), a(0)=1,a(1)=1,a(2)=3.
 * https://oeis.org/A098182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + a(n-3), a(0)=1, a(1)=1, a(2)=3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 87,
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

seq_t a(idx_t n)
{
#if !defined(__cplusplus)
    /* static cache for C */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
    }

    if (n < cache_len)
        return cache[(size_t)n];

    /* expand cache */
    idx_t old_len = cache_len;
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL)
        return (seq_t)0;   /* allocation failed */
    cache = tmp;
    cache_len = new_len;

    /* compute new terms using seq_t arithmetic only */
    for (idx_t i = old_len; i <= n; ++i) {
        seq_t t = cache[i-1] * (seq_t)3;
        t -= cache[i-2];
        t += cache[i-3];
        cache[i] = t;
    }

    return cache[(size_t)n];
#else
    /* static cache for C++ */
    static std::vector<seq_t> v;
    if (v.empty()) {
        v.resize(3);
        v[0] = (seq_t)1;
        v[1] = (seq_t)1;
        v[2] = (seq_t)3;
    }

    if (n < v.size())
        return v[(size_t)n];

    idx_t old_size = v.size();
    v.resize(n + 1);

    /* compute new terms using seq_t arithmetic only */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t t = v[i-1] * (seq_t)3;
        t -= v[i-2];
        t += v[i-3];
        v[i] = t;
    }

    return v[(size_t)n];
#endif
}
