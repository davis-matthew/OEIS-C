/*
 * A009392  Expansion of e.g.f.: log(1+tanh(x))/cosh(x).
 * https://oeis.org/A009392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n/n! in log(1+tanh(x))/cosh(x)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is generated from the EGF recurrence relations of tanh, sech,
   log(1+tanh) and finally the convolution with sech. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the zero offset directly. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Sizes for arrays.  The index n is an unsigned 64‑bit integer; for
       looping purposes we convert it to size_t which is guaranteed to
       accommodate the values we use here. */
    const size_t N = (size_t)n;
    /* Allocate arrays for the needed EGFs: T (tanh), U (1+tanh), S (sech),
       L (log(1+tanh)) and the final A. */
    seq_t *T = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *U = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *S = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *L = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *A = (seq_t*)calloc(N + 1, sizeof(seq_t));

    /* ------------------------------------------------------------------
       1. tanh series T:  (n+1) * T[n+1] = delta_{n,0} - sum_{k=0}^n binom(n,k) T[k] * T[n-k]
       ------------------------------------------------------------------ */
    T[0] = (seq_t)0;
    if (N >= 1)
        T[1] = (seq_t)1;           /* from the recurrence with n=0 */

    for (size_t i = 1; i <= N - 1; ++i) {
        seq_t binom = (seq_t)1;    /* binom(i,0) */
        seq_t sum   = (seq_t)0;
        for (size_t k = 0; k <= i; ++k) {
            sum += binom * T[k] * T[i - k];
            if (k != i)
                binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
        }
        T[i + 1] = (i == 0 ? (seq_t)1 : (seq_t)0) - sum;
    }

    /* ------------------------------------------------------------------
       2. U(x) = 1 + tanh(x).  U[0] = 1, U[k] = T[k] for k >= 1
       ------------------------------------------------------------------ */
    U[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i)
        U[i] = T[i];

    /* ------------------------------------------------------------------
       3. sech series S:  (n+1) * S[n+1] = - sum_{k=0}^n binom(n,k) T[k] * S[n-k]
       ------------------------------------------------------------------ */
    S[0] = (seq_t)1;
    for (size_t i = 0; i <= N - 1; ++i) {
        seq_t binom = (seq_t)1;    /* binom(i,0) */
        seq_t sum   = (seq_t)0;
        for (size_t k = 0; k <= i; ++k) {
            sum += binom * T[k] * S[i - k];
            if (k != i)
                binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
        }
        S[i + 1] = -sum;
    }

    /* ------------------------------------------------------------------
       4. L(x) = log(1 + tanh(x))  so that U = exp(L)
          Recurrence:  L[n+1] = U[n+1] - sum_{k=0}^{n-1} binom(n,k) L[k+1] * U[n-k]
       ------------------------------------------------------------------ */
    L[0] = (seq_t)0;
    for (size_t i = 0; i <= N - 1; ++i) {
        seq_t sum   = (seq_t)0;
        if (i > 0) {
            seq_t binom = (seq_t)1;   /* binom(i,0) */
            for (size_t k = 0; k <= i - 1; ++k) {
                sum += binom * L[k + 1] * U[i - k];
                if (k != i - 1)
                    binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
            }
        }
        L[i + 1] = U[i + 1] - sum;
    }

    /* ------------------------------------------------------------------
       5. Final series A = L * sech = convolution of EGFs
          A[n] = sum_{k=0}^n binom(n,k) L[k] * S[n-k]
       ------------------------------------------------------------------ */
    A[0] = (seq_t)0;
    for (size_t i = 1; i <= N; ++i) {
        seq_t binom = (seq_t)1;    /* binom(i,0) */
        seq_t sum   = (seq_t)0;
        for (size_t k = 0; k <= i; ++k) {
            sum += binom * L[k] * S[i - k];
            if (k != i)
                binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
        }
        A[i] = sum;
    }

    seq_t result = A[N];

    /* Free the temporary storage. */
    free(T);
    free(U);
    free(S);
    free(L);
    free(A);

    return result;
}
