/*
 * A151859  a(0)=0; a(1)=0; a(2)=0; for n>=3 if n=2^i + j with 0<=j<2^i then
 *          a(n)=2*a(j) + a(j + 1) except we add 1 if j=2^i-1.
 * https://oeis.org/A151859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=0 for n=0,1,2; for n≥3, if n=2^i+j with 0≤j<2^i then
 *            a(n)=2*a(j)+a(j+1) (add 1 if j=2^i−1)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* memoised values */
static size_t cache_len = 0;    /* distance one beyond last stored index */
static size_t cache_cap = 0;    /* allocated capacity of cache */

seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
    }

    /* Grow cache if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 2) {
            cache[i] = (seq_t)0;
        } else {
            idx_t pow2 = 1ULL << (63 - __builtin_clzll((unsigned long long)i));
            idx_t j = i - pow2;            /* 0 ≤ j < pow2 */
            seq_t val = cache[j] * (seq_t)2 + cache[j + 1];
            if (j == pow2 - 1) val += (seq_t)1;
            cache[i] = val;
        }
    }

    cache_len = n + 1;                 /* all indices ≤ n are now valid */
    return cache[n];
}
