/*
 * A033462  Exponential (or "EXP") transform of squares A000290.
 * https://oeis.org/A033462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = Σ_{k=1}^{n} C(n‑1, k‑1) * k² * a(n‑k)
 *            where C(n‑1, k‑1) is the binomial coefficient (n‑1 choose k‑1).
 *            This follows from the EGF B(x) = exp(∑ₙ≥1 n² xⁿ/n!) and the
 *            derivative identity f' = c' f for f = exp(c)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t known_n = 0;   /* largest index computed so far */

    /* base case */
    if (n == 0)
        return (seq_t)1;

    /* grow cache if necessary */
    if (n > known_n) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;  /* allocation failed */

        /* copy old values */
        if (cache) {
            for (idx_t i = 0; i <= known_n; ++i)
                new_cache[i] = cache[i];
        }

        /* ensure the first value a[0] = 1 */
        if (known_n == 0)   /* first time we create the array */
            new_cache[0] = (seq_t)1;

        /* compute new entries */
        for (idx_t m = known_n + 1; m <= n; ++m) {
            seq_t sum   = 0;
            seq_t binom = 1;  /* binom(m-1, 0) */

            for (idx_t k = 1; k <= m; ++k) {
                seq_t kk   = (seq_t)k;
                seq_t term = binom * (kk * kk) * new_cache[m - k];
                sum += term;

                if (k < m) {
                    seq_t factor = (seq_t)(m - k);
                    binom = binom * factor / kk;  /* exact division */
                }
            }
            new_cache[m] = sum;
        }

        /* replace old cache */
        free(cache);
        cache    = new_cache;
        known_n  = n;
    }

    return cache[n];
}
