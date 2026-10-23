/*
 * A018212  Alkane (or paraffin) numbers l(11,n).
 * https://oeis.org/A018212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = - Σ_{k=1}^{13} c_k * a(n-k) where c = [1, -5, 6, 10, -29,
 *            9, 36, -36, -9, 29, -10, -6, 5, -1] (c[0] is unused, c[1] = -5
 *            etc.) Initial values a(0) … a(12) are given in OEIS
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* coefficients of the polynomial (1-x)^5*(1-x^2)^4, index 0 unused */
static const int _coef[14] = {
    0,          /* unused, 0‑index placeholder */
    -5,  6, 10, -29,  9,
    36, -36,  -9, 29, -10,
   -6,   5,  -1
};

/* first 13 terms of the sequence, a(0) … a(12)   */
static const seq_t _init_a[13] = {
    (seq_t)1,  (seq_t)5,  (seq_t)25,  (seq_t)85,  (seq_t)255,
    (seq_t)651, (seq_t)1519, (seq_t)3235, (seq_t)6470,
    (seq_t)12190, (seq_t)21942, (seq_t)37854, (seq_t)63090
};

seq_t a(idx_t n)
{
    /* static dynamic cache */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = 13;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out of memory: return 0 */
        for (size_t i = 0; i < 13; ++i)
            cache[i] = _init_a[i];
    }

    /* if n is already cached, return it */
    if (n < cache_size)
        return cache[(size_t)n];

    /* grow cache to size n+1 */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;  /* out of memory: return 0 */
    cache = tmp;
    cache_size = new_size;

    /* compute terms from old_size up to n */
    for (size_t i = 13; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)0;
        /* recurrence: a(i) = -Σ_{k=1}^{13} c_k * a(i-k) */
        for (int k = 1; k <= 13; ++k) {
            sum += (seq_t)(- _coef[k]) * cache[i - k];
        }
        cache[i] = sum;
    }

    return cache[(size_t)n];
}
