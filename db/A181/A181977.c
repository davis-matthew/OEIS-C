/*
 * A181977  Expansion of b(q) * c(q^3)^2 / 9 in powers of q where b(), c() are
 *          cubic AGM theta functions.
 * https://oeis.org/A181977
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient at q^(n-2) of ∏_{k≥1} (1 – q^k)^{c(k)}, where
 *            c(k) = –3 if k≡1,2,4,5,7,8 (mod 9), 0 if k≡3,6 (mod 9), –6 if
 *            k≡0 (mod 9). Compute via the Euler transform recurrence: g[0] =
 *            1 n * g[n] = Σ_{k=1}^{n} w[k] * g[n-k], for n≥1 where w[k] =
 *            Σ_{d|k} d * c(d). Then a(n) = g[n-2] for n≥2
 * Range:     i8 n <= 8, i16 n <= 46, i32 n <= 2041, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 2 – return 0 safely. */
    if (n < 2) {
        return 0;
    }

    /* Compute coefficients up to degree n-2. */
    idx_t N = n - 2;                /* largest index needed in g */
    seq_t *w = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *g = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!w || !g) {
        free(w); free(g);
        return 0;                    /* allocation failed */
    }

    /* Precompute w[k] = Σ_{d|k} d * c(d). */
    for (idx_t d = 1; d <= N; ++d) {
        int mod = d % 9;
        int val;                       /* c(d) value as int */
        switch (mod) {
            case 1: case 2: case 4: case 5: case 7: case 8:
                val = -3; break;
            case 0:
                val = -6; break;
            case 3: case 6:
                val = 0; break;
            default:
                val = 0; /* unreachable */ break;
        }
        if (val == 0) continue;

        seq_t term = (seq_t)d * (seq_t)val;   /* d * c(d) */
        for (idx_t m = d; m <= N; m += d) {
            w[m] += term;
        }
    }

    /* Euler transform recurrence to generate g[n]. */
    g[0] = 1;                         /* constant term of the product */
    for (idx_t nidx = 1; nidx <= N; ++nidx) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= nidx; ++k) {
            sum += w[k] * g[nidx - k];
        }
        g[nidx] = sum / (seq_t)nidx; /* division is exact */
    }

    seq_t result = g[N];               /* a(n) = coefficient of q^(n-2) */
    free(w);
    free(g);
    return result;
}
