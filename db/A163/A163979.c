/*
 * A163979  a(n) = n*(n-1) + A144437(n+2).
 * https://oeis.org/A163979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) initial
 *            values: a(0)=1, a(1)=3, a(2)=5, a(3)=7, a(4)=15
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of computed terms */
static seq_t *cache          = NULL;    /* array of terms */
static idx_t cache_len       = 0;       /* number of terms stored so far (cache_len-1 is max valid index) */
static size_t cache_cap      = 0;       /* allocated capacity */

/* ensure the cache contains terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* increase capacity if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;                    /* assume allocation succeeds */
        cache_cap = new_cap;
    }

    /* fill missing values */
    if ((size_t)n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)          cache[i] = (seq_t)1;
            else if (i == 1)     cache[i] = (seq_t)3;
            else if (i == 2)     cache[i] = (seq_t)5;
            else if (i == 3)     cache[i] = (seq_t)7;
            else if (i == 4)     cache[i] = (seq_t)15;
            else                 cache[i] = 2*cache[i-1] - cache[i-2] + cache[i-3] - 2*cache[i-4] + cache[i-5];
        }
        cache_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
