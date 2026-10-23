/*
 * A151552  G.f.: Product_{k>=1} (1 + x^(2^k-1) + x^(2^k)).
 * https://oeis.org/A151552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 1 for 0 ≤ n ≤ 3; for n ≥ 4, let n = 2^i + j with 0 ≤ j <
 *            2^i. If n+1 or n+2 is a power of two then a(n)=1, else
 *            a(n)=a(j)+a(j+1)
 * Range:     u8 n <= 2037, u16 n <= 9990, u32 n <= 9990, u64 n <= 9990,
 *            u128 n <= 9990, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* External types supplied by the build system */
extern unsigned long long format; /* placeholder, unused */

/* Helper: test whether x is a power of two (x > 0). */
static inline bool is_pow2(idx_t x)
{
    return x != 0 && (x & (x - 1)) == 0;
}

/* Helper: floor(log2(x)) for x > 0 using compiler built‑in. */
static inline int floor_log2_u64(idx_t x)
{
    return 63 - __builtin_clzll(x);
}

/* Static cache for memoisation. */
static seq_t *cache = NULL;        /* pointer to cache array          */
static size_t cache_cap = 0;       /* allocated capacity in elements   */
static idx_t cache_len = 0;        /* smallest index not yet computed */

/* Ensure the cache can store indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) abort();         /* allocation failure */
        cache = p;
        cache_cap = new_cap;
    }
}

/* Return the n‑th term of A151552. */
seq_t a(idx_t n)
{
    if (n < cache_len)                /* already computed */
        return cache[n];

    ensure_cache(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 3) {
            cache[i] = (seq_t)1;
        } else if (is_pow2(i + 1) || is_pow2(i + 2)) {
            cache[i] = (seq_t)1;
        } else {
            int k = floor_log2_u64(i);        /* 2^k ≤ i < 2^(k+1) */
            idx_t p2 = ((idx_t)1) << k;        /* 2^k   */
            idx_t j = i - p2;                  /* 0 ≤ j < 2^k */
            cache[i] = cache[j] + cache[j + 1];
        }
    }

    cache_len = n + 1;
    return cache[n];
}
