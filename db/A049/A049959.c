/*
 * A049959  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 2, and a(3) = 3.
 * https://oeis.org/A049959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049959
 *
 * a(1) = 1, a(2) = 2, a(3) = 3
 * For n ≥ 4:
 *   let k = n − 1
 *   let p be the unique integer with 2^p < k ≤ 2^(p+1)
 *   m = 2*n − 2 − 2^(p+1)
 *   a(n) = (a(1)+⋯+a(n−1)) + a(m)
 *
 * This implementation memoises all values up to the largest `n` requested.
 * It works for any seq_t/idx_t defined in the build.
 *
 * Note: no external dependencies are used.
 */

#include <stddef.h>
#include <stdint.h>

/* static state for memoisation */
static seq_t *cache   = NULL;     /* 1‑based: cache[i] = a(i)   */
static seq_t *prefix  = NULL;     /* prefix[i] = a(1)+…+a(i) */
static size_t cap     = 0;        /* allocated capacity (≥ last+1) */
static idx_t  last    = 0;        /* highest index already computed */

/* floor(log2(x)) for x ≥ 1, using compiler builtin */
static inline idx_t log2_floor(idx_t v)
{
    return 63 - __builtin_clzll((unsigned long long)v);
}

/* ensure the storage can hold indices up to n */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cap) return;   /* already enough space */

    size_t newcap = cap ? cap : 8;
    while (newcap <= (size_t)n + 1) newcap <<= 1;

#ifdef __cplusplus
    seq_t *new_cache  = new seq_t[newcap];
    seq_t *new_prefix = new seq_t[newcap];
    if (cache) {
        for (size_t i = 0; i <= (size_t)last; ++i) {
            new_cache[i]  = cache[i];
            new_prefix[i] = prefix[i];
        }
        delete [] cache;
        delete [] prefix;
    }
#else
    seq_t *new_cache  = (seq_t *)realloc(cache,  newcap * sizeof(seq_t));
    seq_t *new_prefix = (seq_t *)realloc(prefix, newcap * sizeof(seq_t));
#endif

    cache  = new_cache;
    prefix = new_prefix;
    cap    = newcap;

    if (last == 0) prefix[0] = (seq_t)0;   /* initialise prefix[0] */
}

/* public function: compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; guard against 0 */

    if (n <= last) return cache[n];   /* already computed */

    ensure_capacity(n);

    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1]   = (seq_t)1;
            prefix[1]  = cache[1];
        } else if (i == 2) {
            cache[2]   = (seq_t)2;
            prefix[2]  = prefix[1] + cache[2];
        } else if (i == 3) {
            cache[3]   = (seq_t)3;
            prefix[3]  = prefix[2] + cache[3];
        } else {
            idx_t  k = i - 1;
            idx_t  p = log2_floor(k - 1);                   /* 2^p < k ≤ 2^(p+1) */
            idx_t  pow2 = ((idx_t)1) << (p + 1);           /* 2^(p+1) */
            idx_t  m = (idx_t)(2 * i - 2 - pow2);           /* m < i */

            seq_t sum_prev = prefix[i - 1];
            cache[i] = sum_prev + cache[m];
            prefix[i] = sum_prev + cache[i];
        }
    }

    last = n;
    return cache[n];
}
