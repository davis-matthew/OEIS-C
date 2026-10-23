/*
 * A183587  1/24 the number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock
 *          containing exactly three distinct values.
 * https://oeis.org/A183587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) - 30*a(n-2) - 296*a(n-3) + 192*a(n-4) Initial
 *            values: a(1)=56, a(2)=1243, a(3)=27380, a(4)=603766
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Cache to store computed values. */
static seq_t *cache = NULL;      /* array of a(i) with cache[0] = a(1) */
static size_t cache_len = 0;     /* number of elements stored (>=0) */
static size_t cache_cap = 0;     /* allocated capacity */

static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return; /* already computed */

    /* Initialize cache with 4 known terms if it's the first call */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort(); /* abort on allocation failure */
        cache[0] = (seq_t)56;
        cache[1] = (seq_t)1243;
        cache[2] = (seq_t)27380;
        cache[3] = (seq_t)603766;
        cache_len = 4;
    }

    /* Grow cache as needed */
    while (cache_len < n) {
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute a(n) for n = cache_len + 1 (1‑based) */
        size_t idx = cache_len + 1; /* n value to compute */
        seq_t val = 24 * cache[idx - 2]          /* a(n-1) */
                  - 30 * cache[idx - 3]          /* a(n-2) */
                  - 296 * cache[idx - 4]         /* a(n-3) */
                  + 192 * cache[idx - 5];        /* a(n-4) */
        cache[cache_len] = val;
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n=0, return 0 */

    ensure_cache(n);
    return cache[n - 1]; /* array index 0 = a(1) */
}
