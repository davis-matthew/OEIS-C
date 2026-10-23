/*
 * A127690  a(1)=3; for n>1, a(n) is such that a(1)^2+...+a(n)^2 = (1+a(n))^2.
 * https://oeis.org/A127690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3, a(2)=4, for n>2: a(n)=((a(n-1)+1)^2-1)/2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ---------- cache ---------- */

#ifdef __cplusplus
static std::vector<seq_t> cache;  /* cache[i] holds a(i+1) */
#else
static seq_t *cache = NULL;       /* cache[i] holds a(i+1) */
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* enlarge cache capacity to at least 'needed' elements */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}
#endif

/* ---------- sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for the sequence; return 0 */
        return 0;

#ifdef __cplusplus
    /* ensure cache contains values up to n */
    size_t cur = cache.size();
    if (cur < (size_t)n) {
        cache.reserve((size_t)n);
        while (cache.size() < (size_t)n) {
            idx_t i = (idx_t)cache.size() + 1;   /* 1‑based position */
            seq_t val;
            if (i == 1) {
                val = 3;
            } else if (i == 2) {
                val = 4;
            } else {
                seq_t prev = cache[i - 2];       /* a(i-1) */
                seq_t t = prev + 1;              /* (a(i-1)+1) */
                t = t * t;                       /* square */
                t = t - 1;                       /* subtract 1 */
                val = t / 2;                     /* divide by 2 */
            }
            cache.push_back(val);
        }
    }
    return cache[n - 1];
#else
    /* ensure cache contains values up to n */
    if (cache_size < (size_t)n) {
        ensure_cache_capacity((size_t)n);
        for (idx_t i = (idx_t)cache_size + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = 3;
            } else if (i == 2) {
                val = 4;
            } else {
                seq_t prev = cache[i - 2];       /* a(i-1) */
                seq_t t = prev + 1;              /* (a(i-1)+1) */
                t = t * t;                       /* square */
                t = t - 1;                       /* subtract 1 */
                val = t / 2;                     /* divide by 2 */
            }
            cache[cache_size] = val;
            ++cache_size;
        }
    }
    return cache[n - 1];
#endif
}
