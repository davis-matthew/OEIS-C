/*
 * A198243  G.f.: q-Sinh(x) evaluated at q=-x.
 * https://oeis.org/A198243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in * Σ_{m≥0} x^{(m+1)(2m+1)} (1+x) /
 *            Π_{k=1}^{2m+1} (1-(-x)^k)
 * Range:     i8 n <= 48, i16 n <= 67, i32 n <= 67, i64 n <= 67, i128 n <= 67,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binom_n_k.h"

static idx_t cache_max = 0;
static seq_t *cache = NULL;

/* Compute all terms up to 'limit' (inclusive) */
static void compute_cache(idx_t limit)
{
    /* Allocate a fresh cache array (zeroed) */
    seq_t *new_cache = (seq_t*)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!new_cache) abort();                 /* should not happen */

    /* Loop over all possible m (here r = m) with (m+1)(2m+1) ≤ limit */
    for (idx_t r = 0; ; ++r) {
        idx_t D = (r + 1) * (2 * r + 1);     /* shift of the m‑th term   */
        if (D > limit) break;

        idx_t maxdeg = limit - D;           /* terms of F(x) that we need   */
        idx_t K = 2 * r + 1;                /* number of factors in F   */

        /* Temporary arrays for the generating function of 1 / Π(1-(-x)^k)   */
        seq_t *f   = (seq_t*)calloc((size_t)(maxdeg + 1), sizeof(seq_t));
        seq_t *tmp = (seq_t*)calloc((size_t)(maxdeg + 1), sizeof(seq_t));
        seq_t *gconv = (seq_t*)calloc((size_t)(maxdeg + 1), sizeof(seq_t));
        if (!f || !tmp || !gconv) abort();

        /* f[0] = 1, others already zero by calloc */
        f[0] = (seq_t)1;

        seq_t *cur = f;
        seq_t *nxt = tmp;

        /* Multiply through the K factors 1/(1-(-x)^k)   */
        for (idx_t k = 1; k <= K; ++k) {
            seq_t sign = (k & 1) ? (seq_t)-1 : (seq_t)1;   /* (-1)^k   */
            for (idx_t i = 0; i <= maxdeg; ++i) {
                if (i < k) {
                    nxt[i] = cur[i];
                } else {
                    nxt[i] = cur[i] + sign * nxt[i - k];
                }
            }
            /* swap cur and nxt for next factor */
            seq_t *tmp_ptr = cur; cur = nxt; nxt = tmp_ptr;
        }
        /* After the loop 'cur' contains F(x) up to degree maxdeg */

        /* Binomial coefficients for (1+x)^K   */
        seq_t *binom = (seq_t*)malloc((size_t)(K + 1) * sizeof(seq_t));
        if (!binom) abort();

        for (idx_t m = 0; m <= K; ++m)
            binom[m] = binom_n_k(K, m);

        /* Convolution F(x) * (1+x)^K  (stored in gconv)   */
        for (idx_t i = 0; i <= maxdeg; ++i) gconv[i] = (seq_t)0;
        for (idx_t i = 0; i <= maxdeg; ++i) {
            seq_t val = cur[i];
            if (val == (seq_t)0) continue;
            for (idx_t m = 0; m <= K && i + m <= maxdeg; ++m) {
                gconv[i + m] += val * binom[m];
            }
        }

        /* Sign factor (-1)^r   */
        seq_t sign_r = (r & 1) ? (seq_t)-1 : (seq_t)1;

        /* Accumulate into the global cache   */
        for (idx_t i = 0; i <= maxdeg; ++i) {
            idx_t idx = D + i;
            new_cache[idx] += sign_r * gconv[i];
        }

        /* Free temporary arrays */
        free(f);
        free(tmp);
        free(gconv);
        free(binom);
    }

    /* Replace the old cache with the new one */
    free(cache);
    cache = new_cache;
    cache_max = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* term indices start at 1 */
    if (n > cache_max) compute_cache(n);
    return cache[n];
}
