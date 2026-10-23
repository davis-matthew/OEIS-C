/*
 * A196835  Alternating row sums of Sheffer triangle A193685 (5-restricted
 *          Stirling2 numbers).
 * https://oeis.org/A196835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*a(n-1) - Σ_{k=0}^{n-1} binomial(n-1,k)*a(k), a(0)=1
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 26, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation table for previously computed values.  */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated capacity (in entries)      */
static size_t cache_len  = 0;   /* number of values already stored     */

seq_t a(idx_t n)
{
    /* Convert the 64‑bit index to a size_t for array access.  */
    size_t N = (size_t)n;

    /* Grow the cache if needed.  */
    if (N >= cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 1;
        while (new_cap <= N) new_cap <<= 1;
        /* In C++ realloc returns void*, so cast explicitly.  */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL && new_cap > 0) abort();   /* out of memory */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to N (inclusive) that have not yet been done.  */
    for (size_t i = cache_len; i <= N; ++i) {
        if (i == 0) {                    /* a(0) = 1 */
            cache[0] = (seq_t)1;
            continue;
        }

        /* sum_{k=0}^{i-1} binomial(i-1,k) * a(k)  */
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;          /* binomial(i-1,0) */
        size_t m    = i - 1;

        for (size_t k = 0; k <= m; ++k) {
            if (k > 0) {
                /* binom = binom * (m-(k-1)) / k  (exact division) */
                binom = binom * (seq_t)(m - (k - 1)) / (seq_t)k;
            }
            sum += binom * cache[k];
        }

        /* recurrence: a(i) = 5*a(i-1) - sum */
        cache[i] = (seq_t)5 * cache[i - 1] - sum;
    }

    cache_len = N + 1;        /* we now have values 0..N stored */
    return cache[N];
}
