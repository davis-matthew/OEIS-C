/*
 * A151869  a(0)=0; a(1)=1; a(2)=2; for n>=3 if n=2^i + j with 0<=j<2^i then
 *          a(n)=2*a(j) + 2*a(j + 1) except we add 1 if j=2^i-1.
 * https://oeis.org/A151869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=2; for n≥3, write n = 2^i + j with 0≤j<2^i.
 *            Then a(n) = 2*a(j) + 2*a(j+1) + (j==2^i-1 ? 1 : 0)
 * Range:     u8 n <= 57, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute floor(log2(v)) for v≥1. */
static inline idx_t floor_log2_custom(idx_t v)
{
    idx_t r = 0;
    while (v > 1U) {
        v >>= 1;
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;      /* capacity of the cache */
    static idx_t  max_n   = 0;      /* largest index computed so far */

    /* Ensure the cache is large enough. */
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort the call. */
            if (cache) free(cache);
            cache = NULL;
            cap   = 0;
            max_n = 0;
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first term if this is the first call. */
    if (cap > 0 && max_n == 0 && cache[0] != (seq_t)0)
        cache[0] = (seq_t)0;

    /* Compute values up to n (inclusive). */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        if (k == 1) {
            cache[1] = (seq_t)1;
            continue;
        }
        if (k == 2) {
            cache[2] = (seq_t)2;
            continue;
        }

        /* n = 2^i + j, 0 <= j < 2^i */
        idx_t i   = floor_log2_custom(k);
        idx_t pow = (idx_t)1 << i;
        idx_t j   = k - pow;

        seq_t val = (seq_t)2 * cache[j] + (seq_t)2 * cache[j + 1];
        if (j == (pow - 1U))
            val += (seq_t)1;

        cache[k] = val;
    }

    max_n = n;
    return cache[n];
}
