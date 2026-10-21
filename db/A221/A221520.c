/*
 * A221520  Number of 0..4 arrays of length n with each element differing from
 *          at least one neighbor by 2 or more.
 * https://oeis.org/A221520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) + 6*a(n-3) + 4*a(n-4) + 4*a(n-6) (n
 *            >= 7). Initial values: a(1)=0, a(2)=12, a(3)=30, a(4)=144,
 *            a(5)=536, a(6)=2172
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;    /* cache[0] holds a(1) */
#else
static seq_t *cache = NULL;         /* cache[0] holds a(1) */
static idx_t cache_len = 0;         /* number of terms stored so far */
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* offset starts at 1; undefined for n<1 */

#ifdef __cplusplus
    if (cache.size() < n) {
        cache.reserve(n);
        for (idx_t i = cache.size(); i < n; ++i) {
            idx_t idx = i + 1;     /* 1‑based term index */
            seq_t val;
            if (idx == 1)          val = (seq_t)0;
            else if (idx == 2)     val = (seq_t)12;
            else if (idx == 3)     val = (seq_t)30;
            else if (idx == 4)     val = (seq_t)144;
            else if (idx == 5)     val = (seq_t)536;
            else if (idx == 6)     val = (seq_t)2172;
            else {
                val = cache[idx - 2] * (seq_t)2;      /* 2*a(idx-1) */
                val += cache[idx - 3] * (seq_t)6;      /* 6*a(idx-2) */
                val += cache[idx - 4] * (seq_t)6;      /* 6*a(idx-3) */
                val += cache[idx - 5] * (seq_t)4;      /* 4*a(idx-4) */
                val += cache[idx - 7] * (seq_t)4;      /* 4*a(idx-6) */
            }
            cache.push_back(val);
        }
    }
    return cache[n - 1];
#else
    if (cache_len < n) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache)
            abort(); /* memory allocation failed */
        cache = new_cache;
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)          val = (seq_t)0;
            else if (i == 2)     val = (seq_t)12;
            else if (i == 3)     val = (seq_t)30;
            else if (i == 4)     val = (seq_t)144;
            else if (i == 5)     val = (seq_t)536;
            else if (i == 6)     val = (seq_t)2172;
            else {
                val = cache[i - 2] * (seq_t)2;          /* 2*a(i-1) */
                val += cache[i - 3] * (seq_t)6;          /* 6*a(i-2) */
                val += cache[i - 4] * (seq_t)6;          /* 6*a(i-3) */
                val += cache[i - 5] * (seq_t)4;          /* 4*a(i-4) */
                val += cache[i - 7] * (seq_t)4;          /* 4*a(i-6) */
            }
            cache[i - 1] = val;
        }
        cache_len = n;
    }
    return cache[n - 1];
#endif
}
