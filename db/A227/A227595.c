/*
 * A227595  Expansion of phi(-x) * psi(x^3)^2 / chi(-x^3) in powers of x where
 *          phi(), psi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A227595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; * For n≥1: * let b_d = periodic pattern
 *            {-2,-1,1,-1,-2,-3}, d≥1 (period 6) * let A_k = Σ_{d|k} d*b_d
 *            (integer) * then a(n) = (1/n) * Σ_{k=1..n} A_k * a(n‑k) * This
 *            is the Euler transform recurrence for the sequence
 * Range:     i8 n <= 18, i16 n <= 774, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const int pat[6] = {-2, -1, 1, -1, -2, -3};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0; /* number of elements stored = cap */

    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (new_cache == NULL) return 0; /* allocation failed, return 0 */

        /* copy old values */
        if (cache != NULL && cap > 0)
            memcpy(new_cache, cache, cap * sizeof(seq_t));

        /* compute new A array */
        seq_t *A = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (A == NULL) { free(new_cache); return 0; }

        /* fill A[k] = Σ_{d|k} d*b_d */
        for (idx_t d = 1; d < new_cap; ++d) {
            seq_t d_mul = (seq_t)d * (seq_t)pat[(d - 1) % 6];
            for (idx_t j = d; j < new_cap; j += d)
                A[j] += d_mul;
        }

        /* compute a values using recurrence */
        new_cache[0] = (seq_t)1;
        for (idx_t i = 1; i < new_cap; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k)
                sum += A[k] * new_cache[i - k];
            new_cache[i] = sum / (seq_t)i;
        }

        free(A);
        if (cache != NULL) free(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    return cache[n];
}
