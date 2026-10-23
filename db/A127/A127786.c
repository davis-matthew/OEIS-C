/*
 * A127786  Expansion of phi(q) * phi(q^2) * phi(-q^4) in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A127786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 816, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Coefficients of φ(q) * φ(q²) * φ(−q⁴).  The triple convolution
   is performed on three sparse power series:
       φ(q)     = 1 + 2∑_{s≥1} q^{s²}
       φ(q²)    = 1 + 2∑_{s≥1} q^{2s²}
       φ(−q⁴)   = 1 + 2∑_{s≥1} (−1)^{s} q^{4s²}
   The function builds the sequence up to the requested index n and
   caches all coefficients for subsequent calls. */
seq_t a(idx_t n)
{
    /* Cached values.  Size is always at least n+1. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));

        /* 3‑fold convolution.  The series are extremely sparse,
           only terms at perfect squares or twice/Z/4 times squares. */
        for (idx_t s1 = 0; s1 * s1 <= n; ++s1) {
            idx_t exp1 = s1 * s1;
            seq_t coeff1 = (s1 == 0 ? 1 : 2);

            for (idx_t s2 = 0; ; ++s2) {
                idx_t exp2 = 2 * s2 * s2;
                if (exp1 + exp2 > n) break;
                seq_t coeff2 = (s2 == 0 ? 1 : 2);

                for (idx_t s3 = 0; ; ++s3) {
                    idx_t exp3 = 4 * s3 * s3;
                    if (exp1 + exp2 + exp3 > n) break;
                    seq_t coeff3 = (s3 == 0 ? 1
                                         : 2 * ((s3 & 1) ? -1 : 1));

                    new_cache[exp1 + exp2 + exp3] += coeff1 * coeff2 * coeff3;
                }
            }
        }

        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
