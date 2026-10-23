/*
 * A194453  E.g.f. satisfies: A(x) = exp(x) - sqrt(1 - A(x)^2).
 * https://oeis.org/A194453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A194453 – Exponential generating function  
 * A(x) = exp(x) - sqrt(1 - A(x)^2)    
 * 
 * Recurrence (OEIS):
 *   a(1) = 1
 *   a(n) = 2^{n-1} + Σ_{j=1}^{n-1} C(n,j) · a(j) · (a(n-j) – 1)
 *        (n ≥ 2)
 * where C(n,j) = binomial coefficient.
 * The function caches previously computed values and supports non‑
 * monotonically increasing calls to a(n).
 */

#include <stdlib.h>

/* external integer types supplied by the build */
extern seq_t a(idx_t n);

/* implementation */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* a(0), a(1), ... */
    static seq_t *pow2_cache = NULL;   /* 2^{i-1} for each i */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static size_t cache_len  = 0;      /* number of initialized elements */

    /* cast index to size_t for array access; assumes n fits into size_t */
    size_t idx = (size_t)n;
    if (idx == 0) return 0;   /* not used for offset 1, but safe */

    /* initial allocation for indices 0 and 1 */
    if (cache == NULL) {
        cache_cap = 3;   /* accommodate indices 0,1,2 as minimal size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure */
        pow2_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!pow2_cache) { free(cache); return 0; }

        cache[0] = 0;       /* a(0)  – not part of the sequence but kept for
                               ease of indexing in the recurrence        */
        cache[1] = 1;       /* a(1)  – first term of the sequence */
        pow2_cache[1] = (seq_t)1;  /* 2^{1-1} = 1 */
        cache_len = 2;
    }

    /* enlarge capacity if necessary */
    size_t needed = idx + 1;   /* need indices 0..idx */
    if (needed > cache_cap) {
        size_t new_cap = needed * 2;
        if (new_cap < needed) new_cap = needed;  /* safety for overflow */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure */
        cache = tmp;

        tmp = (seq_t *)realloc(pow2_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        pow2_cache = tmp;

        cache_cap = new_cap;
    }

    /* compute missing terms up to idx */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i == 0) { cache[i] = 0; continue; }
        if (i == 1) { cache[i] = 1; continue; }

        /* compute 2^{i-1} */
        pow2_cache[i] = pow2_cache[i-1] * ((seq_t)2);

        /* recurrence sum */
        seq_t sum = 0;
        seq_t C   = (seq_t)i;        /* C(i,1) */
        for (size_t j = 1; j <= i-1; ++j) {
            seq_t term = cache[j] * (cache[i - j] - 1);
            term *= C;
            sum += term;

            /* update binomial coefficient C(i,j+1) */
            if (j < i-1) {
                C = C * ((seq_t)(i - j)) / ((seq_t)(j + 1));
            }
        }
        cache[i] = sum + pow2_cache[i];
    }

    cache_len = idx + 1;   /* mark all indices up to idx as initialized */
    return cache[idx];
}
