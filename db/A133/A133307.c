/*
 * A133307  a(n) = (1/n)*Sum_{i=0..n-1} C(n,i)*C(n,i+1)*6^i*7^(n-i), a(0)=1.
 * https://oeis.org/A133307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6 * Σ_{k=0}^{n-1} a(k) * a(n-1-k), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A133307 using its convolution recurrence. */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n) {
    /* Cache of previously computed values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_len = 0;    /* number of computed values */

    /* Ensure space for a(n). */
    if (cache_len <= n) {
        size_t required = (size_t)(n + 1);

        if (required > cache_cap) {
            size_t new_cap = cache_cap == 0 ? 1 : cache_cap;
            while (new_cap < required) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache      = tmp;
            cache_cap  = new_cap;
        }

        /* Compute missing values up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;          /* a(0) */
            } else {
                seq_t sum = 0;
                idx_t m = i - 1;               /* sum over k = 0 .. n-1 */
                for (idx_t k = 0; k <= m; ++k) {
                    sum += cache[k] * cache[m - k];
                }
                cache[i] = cache[i - 1] + (seq_t)6 * sum;  /* recurrence */
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
