/*
 * A054891  Cusp form of weight 13/2 associated to the unique cusp form of
 *          weight 12 under Shimura correspondence.
 * https://oeis.org/A054891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ((8 * θ3^4 - θ2^4) * θ3 * (θ2 * θ4)^4) / 128 where θj are
 *            Jacobi theta functions. The implementation computes the
 *            q‑expansion up to the requested index using convolution of
 *            integer sequences
 * Range:     i8 none, i16 n <= 11, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   conv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/conv.h"

seq_t a(idx_t n)
{
    /* Cached results can be reused for subsequent calls */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;

    if (n == 0) return 0;   /* undefined, but safe return */
    if (n > cache_max) {
        idx_t N = n;                          /* maximum index needed   */
        size_t len = (size_t)(N + 1);          /* array length          */

        /* Allocate all helper arrays */
        seq_t *t3  = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *f   = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *t4  = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *H   = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *H2  = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *H4  = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *t32 = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *S1  = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *f2  = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *F4  = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *A   = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *X   = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *S4  = (seq_t*)calloc(len, sizeof(seq_t));
        seq_t *Y   = (seq_t*)calloc(len, sizeof(seq_t));

        /* 1. θ3(q) : coefficient 1 at q^0, 2 at q^(k^2) (k>0) */
        t3[0] = 1;
        for (idx_t m = 1; m * m <= N; ++m)
            t3[m * m] = 2;

        /* 2. f(q) = θ2(q)/q^(1/4) : coefficient 2 at q^(k(k+1)) */
        for (idx_t k = 0; k * (k + 1) <= N; ++k)
            f[k * (k + 1)] = 2;

        /* 3. θ4(q) : coefficient 1 at q^0, 2*(-1)^m at q^(m^2) (m>0) */
        t4[0] = 1;
        for (idx_t m = 1; m * m <= N; ++m)
            t4[m * m] = (seq_t)(2 * ((m % 2 == 0) ? 1 : -1));

        /* 4. H = f * θ4          (convolution) */
        conv(f, t4, H, N);

        /* 5. H^2 and H^4          */
        conv(H, H, H2, N);
        conv(H2, H2, H4, N);

        /* 6. θ3^2 and θ3^4          */
        conv(t3, t3, t32, N);
        conv(t32, t32, S1, N);

        /* 7. f^2 and f^4             */
        conv(f, f, f2, N);
        conv(f2, f2, F4, N);          /* F4 is f^4, no shift yet */

        /* 8. Compute A = 8*S1 - shifted(F4)   */
        for (idx_t i = 0; i <= N; ++i) {
            seq_t val = 8 * S1[i];
            if (i >= 1) val -= F4[i - 1];   /* shift by one (q^1)   */
            A[i] = val;
        }

        /* 9. X = A * θ3                  */
        conv(A, t3, X, N);

        /* 10. S4 = shifted(H4)           (H4 starts at q^1) */
        S4[0] = 0;
        for (idx_t i = 1; i <= N; ++i)
            S4[i] = H4[i - 1];

        /* 11. Y = X * S4                  */
        conv(X, S4, Y, N);

        /* 12. Final coefficients: divide by 128 */
        seq_t *new_cache = (seq_t*)calloc((size_t)n, sizeof(seq_t));
        for (idx_t i = 1; i <= N; ++i)
            new_cache[i - 1] = Y[i] / 128;

        /* Replace old cache */
        if (cache) free(cache);
        cache = new_cache;
        cache_max = n;

        /* Free temporary arrays */
        free(t3); free(f); free(t4);
        free(H); free(H2); free(H4);
        free(t32); free(S1);
        free(f2); free(F4);
        free(A); free(X); free(S4); free(Y);
    }

    return cache[n - 1];
}
