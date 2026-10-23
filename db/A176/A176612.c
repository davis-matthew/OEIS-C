/*
 * A176612  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=2, k=1 and l=1.
 * https://oeis.org/A176612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:
 *   a(0) = 1, a(1) = 2;
 *   For n ≥ 1: a(n+1) = Σ_{p=0}^{n} (a(p) * a(n-p) + 1) + 1
 *   Equivalent: a(n+1) = Σ_{p=0}^{n} a(p) * a(n-p) + n + 2
 */

#include <stdlib.h>   /* for abort, malloc, realloc */

static seq_t* cache = NULL;      /* memoisation array */
static idx_t cache_len = 0;      /* number of filled entries (≤ cache_cap) */
static idx_t cache_cap = 0;      /* currently allocated capacity */

static void ensure_cache(idx_t to)
{
    if (cache_cap <= to) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap <= to) new_cap = to + 1;
#ifdef __cplusplus
        seq_t* tmp = new seq_t[new_cap];
        if (cache) {
            /* copy existing values */
            for (idx_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
            delete[] cache;
        }
        cache = tmp;
#else
        seq_t* tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
#endif
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* handle base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* make sure memoisation storage is big enough */
    ensure_cache(n);

    /* initialise base values on first use */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t p = 0; p < i; ++p) {
            sum += cache[p] * cache[i - 1 - p];
        }
        /* a[i] = Σ a[p]*a[i-1-p] + (i-1)+2  →  + (i+1) */
        cache[i] = sum + (seq_t)(i + 1);
    }
    cache_len = n + 1;
    return cache[n];
}
