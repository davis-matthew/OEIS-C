/*
 * A113146  Row 5 of table A113143; equal to INVERT of quintic (or 5-fold)
 *          factorials shifted one place right.
 * https://oeis.org/A113146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n+1) = Σ_{k=0..n} a(k) * f(n-k), where f(0)=1 and
 *            f(m) = f(m-1) * (5*m-4) for m≥1 (f(m) is the m-th 5‑fold
 *            factorial)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed a and f values */
    static idx_t cache_len = 0;
    static seq_t *a_buf = NULL;
    static seq_t *f_buf = NULL;

    if (n < cache_len)
        return a_buf[n];

    /* grow cache to at least n+1 entries */
    idx_t new_len = (cache_len == 0) ? 1 : cache_len;
    while (new_len <= n)
        new_len <<= 1;

    a_buf = (seq_t *)realloc(a_buf, new_len * sizeof(seq_t));
    f_buf = (seq_t *)realloc(f_buf, new_len * sizeof(seq_t));

    /* initialise first element(s) if this is the first call */
    if (cache_len == 0) {
        a_buf[0] = (seq_t)1;
        f_buf[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute missing f and a values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* f[i] = f[i-1] * (5*i-4) */
        seq_t factor = (seq_t)5 * (seq_t)i - (seq_t)4;
        f_buf[i] = f_buf[i-1] * factor;

        /* a[i] = Σ_{k=0}^{i-1} a[k] * f[i-1-k] */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k)
            sum += a_buf[k] * f_buf[i-1-k];
        a_buf[i] = sum;
    }

    cache_len = n + 1;
    return a_buf[n];
}
