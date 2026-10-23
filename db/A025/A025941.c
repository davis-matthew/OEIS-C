/*
 * A025941  Expansion of 1/((1-2*x)*(1-3*x)*(1-6*x)*(1-11*x)).
 * https://oeis.org/A025941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1)-157*a(n-2)+432*a(n-3)-396*a(n-4), with a(0)=1,
 *            a(1)=22, a(2)=327, a(3)=4172
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure the cache can hold values up to at least idx n.  */
static void
ensure_capacity(idx_t n)
{
    /* Allocate at least n+1 entries. */
    while (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return; /* Out of memory; ignore for contest purposes. */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t
a(idx_t n)
{
    /* Initialise cache on first call. */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* OOM – return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)327;
        cache[3] = (seq_t)4172;
        cache_len = 4;
    }

    /* Extend cache if required. */
    if (cache_len <= (size_t)n) {
        ensure_capacity(n);
        for (idx_t k = (idx_t)cache_len; k <= n; ++k) {
            seq_t val;
            if (k == 0) {
                val = (seq_t)1;
            } else if (k == 1) {
                val = (seq_t)22;
            } else if (k == 2) {
                val = (seq_t)327;
            } else if (k == 3) {
                val = (seq_t)4172;
            } else {
                val = 22 * cache[k-1]
                    - 157 * cache[k-2]
                    + 432 * cache[k-3]
                    - 396 * cache[k-4];
            }
            cache[(size_t)k] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
