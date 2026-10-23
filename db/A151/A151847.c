/*
 * A151847  a(0)=1; a(1)=0; a(2)=1; for n>=3 if n=2^i + j with 0<=j<2^i then
 *          a(n)=a(j) + a(j + 1) except we add 1 if j=2^i-1.
 * https://oeis.org/A151847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1017, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A151847:  a(0)=1; a(1)=0; a(2)=1;  
    For n≥3 write n = 2^i + j with 0≤j<2^i.  
    Then a(n) = a(j) + a(j+1) + (j==2^i-1?1:0).   */

/*  This function works for any build where   */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#if defined(__cplusplus)
#  include <vector>
#endif

/* floor(log2(n)) for 0<n<2^64 (works for n==0 as 0). */
static inline idx_t floor_log2_u64(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/*  Compute a(n) with incremental memoization.  */
seq_t a(idx_t n)
{
#if defined(__cplusplus)          /* C++ build – use std::vector<seq_t> */
    static std::vector<seq_t> cache;
    size_t start = cache.size();
    if (start <= (size_t)n) {
        cache.resize((size_t)n + 1);
        for (idx_t i = (idx_t)start; i <= n; ++i) {
            if (i == 0) cache[i] = (seq_t)1;
            else if (i == 1) cache[i] = (seq_t)0;
            else if (i == 2) cache[i] = (seq_t)1;
            else {
                idx_t i_log = floor_log2_u64(i);
                idx_t pow   = (idx_t)1 << i_log;   /* 2^i */
                idx_t j     = i - pow;              /* j = n - 2^i */
                seq_t val   = cache[j] + cache[j + 1];
                if (j == pow - 1) ++val;
                cache[i] = val;
            }
        }
    }
    return cache[(size_t)n];
#else                              /* C build – use raw array */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        for (idx_t i = (idx_t)cache_cap; i <= n; ++i) {
            if (i == 0) cache[i] = (seq_t)1;
            else if (i == 1) cache[i] = (seq_t)0;
            else if (i == 2) cache[i] = (seq_t)1;
            else {
                idx_t i_log = floor_log2_u64(i);
                idx_t pow   = (idx_t)1 << i_log;   /* 2^i */
                idx_t j     = i - pow;              /* j = n - 2^i */
                seq_t val   = cache[j] + cache[j + 1];
                if (j == pow - 1) ++val;
                cache[i] = val;
            }
        }
        cache_cap = new_cap;
    }
    return cache[n];
#endif
}
