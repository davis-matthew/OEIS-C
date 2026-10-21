/*
 * A215516  n-th derivative of cos(x)^sin(x) at x=0.
 * https://oeis.org/A215516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * coefficient of x^n in exp( sin(x)*log(cos x) ). The
 *            values are computed by the recurrence: A(0) = 1 A(n) =
 *            Σ_{k=1..n} binom(n-1,k-1) * G(k) * A(n-k) where G(k) = k! *
 *            coefficient of x^k in sin(x)*log(cos x)
 * Range:     i8 n <= 8, i16 n <= 10, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation below builds the required factorial‑scaled
    coefficient arrays by elementary integer arithmetic only.  It works
    for any seq_t type provided by the build (including arbitrary
    precision integers).  It uses only C11 (and also works on C++17). */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* Convert to a standard size_t for array indexing. */
    size_t N = (size_t)n;

    /* Allocate storage for the factorial‑scaled coefficient arrays. */
    seq_t *C = (seq_t *)malloc((N + 1) * sizeof(seq_t));   /* cos scaled          */
    seq_t *D = (seq_t *)malloc((N + 1) * sizeof(seq_t));   /* 1/cos scaled       */
    seq_t *L = (seq_t *)malloc((N + 1) * sizeof(seq_t));   /* log(cos) scaled    */
    seq_t *G = (seq_t *)malloc((N + 1) * sizeof(seq_t));   /* sin*log scaled     */
    seq_t *A = (seq_t *)malloc((N + 1) * sizeof(seq_t));   /* target sequence    */

    /* 1.  Cosine coefficients scaled by factorial. */
    for (size_t i = 0; i <= N; ++i) {
        if (i == 0) {
            C[i] = 1;
        } else if ((i & 1) == 0) {            /* even i */
            /* (-1)^{i/2} */
            if ((i / 2) & 1) C[i] = -1;
            else             C[i] =  1;
        } else {
            C[i] = 0;
        }
    }

    /* 2.  Compute inverse of cosine:  D[0] = 1,
          D[n] = - Σ_{k=1..n} binom(n,k) * C[k] * D[n-k]  */
    D[0] = 1;
    for (size_t n1 = 1; n1 <= N; ++n1) {
        seq_t sum = 0;
        seq_t bin = 1;                     /* binom(n1,1) will be set in loop */
        for (size_t k = 1; k <= n1; ++k) {
            if (k == 1) {
                bin = (seq_t)n1;           /* binom(n1,1) = n1 */
            } else {
                bin = bin * (n1 - k + 1) / (seq_t)k;
            }
            sum += bin * C[k] * D[n1 - k];
        }
        D[n1] = -sum;
    }

    /* 3.  Compute log(cos) scaled:  L[n] = Σ_{i=0..n-1} binom(n-1,i) * C[i+1] * D[n-1-i]   */
    L[0] = 0;  /* log(cos)(0) = 0 */
    for (size_t n1 = 1; n1 <= N; ++n1) {
        seq_t sum = 0;
        seq_t bin = 1;                     /* binom(n1-1,0) */
        for (size_t i = 0; i <= n1 - 1; ++i) {
            if (i != 0) {
                bin = bin * ((n1 - 1) - (i - 1)) / (seq_t)i;
            }
            sum += bin * C[i + 1] * D[n1 - 1 - i];
        }
        L[n1] = sum;
    }

    /* 4.  Compute sin(x) scaled (odd indices only). */
    seq_t *S = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    for (size_t i = 0; i <= N; ++i) {
        if ((i & 1) == 1) {                /* odd i */
            /* (-1)^{(i-1)/2} */
            if (((i - 1) / 2) & 1) S[i] = -1;
            else                  S[i] =  1;
        } else {
            S[i] = 0;
        }
    }

    /* 5.  Compute G = sin * log(cos) scaled:
           G[k] = Σ_{i=0..k} binom(k,i) * S[i] * L[k-i]   */
    G[0] = 0;
    for (size_t k = 1; k <= N; ++k) {
        seq_t sum = 0;
        seq_t bin = 1;                     /* binom(k,0) */
        for (size_t i = 0; i <= k; ++i) {
            if (i != 0) {
                bin = bin * (k - i + 1) / (seq_t)i;
            }
            if (S[i] != 0) {
                sum += bin * S[i] * L[k - i];
            }
        }
        G[k] = sum;
    }

    /* 6.  Compute the target sequence A via the recurrence. */
    A[0] = 1;
    for (size_t n1 = 1; n1 <= N; ++n1) {
        seq_t sum = 0;
        seq_t bin = 1;                     /* binom(n1-1,0) */
        for (size_t k = 1; k <= n1; ++k) {
            if (k != 1) {
                bin = bin * ((n1 - 1) - (k - 2)) / (seq_t)(k - 1);
            }
            /* bin now equals binom(n1-1,k-1) */
            sum += bin * G[k] * A[n1 - k];
        }
        A[n1] = sum;
    }

    seq_t result = A[N];

    /* Free allocated memory. */
    free(C);
    free(D);
    free(L);
    free(G);
    free(A);
    free(S);

    return result;
}
