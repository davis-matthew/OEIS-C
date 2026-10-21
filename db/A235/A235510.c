/*
 * A235510  Number of (n+1) X (1+1) 0..1 arrays with the sum of each 2 X 2
 *          subblock maximum and minimum lexicographically nondecreasing
 *          rowwise and columnwise.
 * https://oeis.org/A235510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-3) + a(n-4) + 2*a(n-5) for n>5 Initial
 *            values (offset 1): a(1)=16, a(2)=58, a(3)=209, a(4)=746,
 *            a(5)=2660
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array of computed terms */
static idx_t cache_len = 0;          /* number of terms stored (>=5 after init) */
static idx_t cache_cap = 0;          /* allocated capacity */

/* Helper: ensure the cache can hold at least n terms (1‑based). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initial allocation: at least 8 terms for safety */
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        /* store first five terms */
        cache[0] = (seq_t)16;
        cache[1] = (seq_t)58;
        cache[2] = (seq_t)209;
        cache[3] = (seq_t)746;
        cache[4] = (seq_t)2660;
        cache_len = 5;
    }
    if (n <= cache_len) return;

    /* resize if necessary */
    while (cache_cap < n) {
        cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    }
    if (cache_cap > cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* indices: cache[k] corresponds to a(k+1) (since 0‑based) */
        seq_t a_nm1  = cache[i - 2];   /* a(i-1) */
        seq_t a_nm3  = (i >= 4) ? cache[i - 4] : 0; /* a(i-3) */
        seq_t a_nm4  = (i >= 5) ? cache[i - 5] : 0; /* a(i-4) */
        seq_t a_nm5  = (i >= 6) ? cache[i - 6] : 0; /* a(i-5) */
        seq_t val = (seq_t)4 * a_nm1
                  - (seq_t)6 * a_nm3
                  +          a_nm4
                  + (seq_t)2 * a_nm5;
        cache[i - 1] = val;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; undefined for n=0 */
    ensure_cache(n);
    return cache[n - 1];
}
