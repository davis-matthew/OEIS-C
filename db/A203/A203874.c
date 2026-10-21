/*
 * A203874  Number of (n+1)X5 0..3 arrays with no 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A203874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 118*a(n-1) - 117*a(n-2) - 34398*a(n-3) + 120834*a(n-4) -
 *            86436*a(n-5) (empirical recurrence), initial terms: a(1)=4375,
 *            a(2)=500095, a(3)=57177415, a(4)=6542729803, a(5)=748675224487
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* 1‑based cache of computed terms */
static size_t cache_len = 0;      /* allocated length (>= n+1) */
static size_t computed_max = 0;   /* greatest n for which cache[n] is valid */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_len = need;
    }
}

/* External linkage, visible to other translation units. */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1. */
    if (n == 0) abort(); /* out of range */

    /* At least the first five terms are needed. */
    idx_t need = n > 5 ? n : 5;
    ensure_cache(need);

    /* Initialise the first five terms once. */
    if (computed_max < 5) {
        cache[1] = (seq_t)4375;
        cache[2] = (seq_t)500095;
        cache[3] = (seq_t)57177415;
        cache[4] = (seq_t)6542729803;
        cache[5] = (seq_t)748675224487;
        computed_max = 5;
    }

    /* If the requested term is already computed, return it. */
    if ((size_t)n <= computed_max) {
        return cache[n];
    }

    /* Compute missing terms up to n using the empirical recurrence. */
    for (idx_t i = (idx_t)(computed_max + 1); i <= n; ++i) {
        seq_t pos = (seq_t)118   * cache[i-1]
                 + (seq_t)120834 * cache[i-4];
        seq_t neg = (seq_t)117   * cache[i-2]
                 + (seq_t)34398  * cache[i-3]
                 + (seq_t)86436  * cache[i-5];
        cache[i] = pos - neg;   /* guaranteed non‑negative */
    }

    computed_max = (size_t)n;
    return cache[n];
}
