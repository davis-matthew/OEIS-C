/*
 * A075834  Coefficients of power series A(x) such that n-th term of A(x)^n =
 *          n! x^(n-1) for n > 0.
 * https://oeis.org/A075834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=1, a(n) = (n-1)*a(n-1) + Σ_{j=2}^{n-2}
 *            (j-1)*a(j)*a(n-j)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Lazy memoisation: cache of a(0..?) and of (j-1)*a(j) */
    static seq_t *cache = NULL;      /* a[0..] */
    static seq_t *c     = NULL;      /* c[j] = (j-1)*a(j) for j ≥ 2 */
    static size_t cache_len = 0;     /* current capacity (≥ next unused index) */

    if (n < cache_len)               /* already computed */
        return cache[n];

    /* Expand capacity to at least n+1 */
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    seq_t *new_c     = (seq_t *)realloc(c,     new_len * sizeof(seq_t));
    if (!new_cache || !new_c) abort();   /* out of memory */

    cache = new_cache;
    c     = new_c;

    /* Compute values from the previously known size up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = 1;
            /* c[i] unused for i<2 */
        } else if (i == 2) {
            cache[i] = 1;
            c[i] = (seq_t)(i-1) * cache[i];
        } else {
            seq_t sum = (seq_t)(i-1) * cache[i-1];
            for (idx_t j = 2; j <= i-2; ++j)
                sum += c[j] * cache[i-j];
            cache[i] = sum;
            c[i] = (seq_t)(i-1) * cache[i];
        }
    }

    cache_len = new_len;
    return cache[n];
}
