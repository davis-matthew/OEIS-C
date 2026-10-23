/*
 * A167598  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167598: Number of reduced words of length n in the Coxeter group
   on 41 generators with relations (S_i)^2 = (S_i S_j)^14 = I.
   The sequence has rational generating function
       P(t)/Q(t)   where
           P(t) = t^14 + 2t^13 + ... + 2t + 1
           Q(t) = 780t^14 - 39t^13 - ... - 39t + 1
   From this, for n ≥ 1 the coefficients satisfy
           a[n] = P[n] + 39⋅∑_{k=1}^{13} a[n‑k] – 780⋅a[n‑14]     (1)
   with P[n] = 1 for n = 0 or 14, P[n] = 2 for 1 ≤ n ≤ 13, P[n] = 0 otherwise.
   The initial value a[0] = 1.  The recurrence is used for all n ≥ 1 and
   automatically generates a[1] … a[13] from the P‑coefficients. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* use a growing static cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of terms already computed (last index + 1) */

    /* adjust cache size if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        if (new_len > cache_cap) {
            size_t new_cap = new_len > cache_cap * 2 ? new_len : (cache_cap ? cache_cap * 2 : 16);
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing terms */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else {
                /* compute the sum of the previous 13 terms */
                seq_t sum13 = 0;
                for (size_t k = 1; k <= 13 && i >= k; ++k)
                    sum13 += cache[i - k];

                /* term from a[i‑14] if it exists */
                seq_t term14 = 0;
                if (i >= 14)
                    term14 = cache[i - 14];

                /* coefficient from the numerator P(t) */
                seq_t Pn = 0;
                if (i == 14)
                    Pn = (seq_t)1;
                else if (i <= 13)
                    Pn = (seq_t)2;
                /* else Pn remains 0 for i > 14 */

                /* recurrence (1) */
                cache[i] = Pn + (seq_t)39 * sum13 - (seq_t)780 * term14;
            }
        }
        cache_len = new_len;
    }
    return cache[(size_t)n];
}
