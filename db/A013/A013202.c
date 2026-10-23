/*
 * A013202  Expansion of e.g.f. log(sech(x) + sin(x)).
 * https://oeis.org/A013202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of a(n) where  a(n) is the integer coefficient in the e.g.f
   expansion of log(sech(x) + sin(x)), i.e.
   log(sech(x)+sin(x)) = Σ_{n≥1} a(n) · x^n / n! */
#include <stdlib.h>

/* Auxiliary macro: absolute value for seq_t (works for signed integer types) */
#define ABS_SEQ_T(x) ((x) < 0 ? -(x) : (x))

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)0;          /* a(0) = 0 */

    /* Allocate space for all auxiliary sequences up to n */
    size_t N = (size_t)n + 1;     /* indices 0 … n */
    seq_t *e      = (seq_t *)malloc(N * sizeof(seq_t));   /* Euler numbers (sech) */
    seq_t *s      = (seq_t *)malloc(N * sizeof(seq_t));   /* h(x)=sech+sin coeffs */
    seq_t *invH   = (seq_t *)malloc(N * sizeof(seq_t));   /* 1 / h(x) coeffs */
    seq_t *a_arr  = (seq_t *)malloc(N * sizeof(seq_t));   /* desired a(n) coeffs */

    if (!e || !s || !invH || !a_arr) {       /* allocation failed */
        free(e); free(s); free(invH); free(a_arr);
        return (seq_t)0;
    }

    /* 1. Euler numbers e[n] (coefficients of sech(x) in e.g.f.) */
    e[0] = (seq_t)1;
    if (N > 1)
        e[1] = (seq_t)0;
    for (idx_t i = 2; i < N; ++i) {
        seq_t sum   = 0;
        idx_t start = i & 1ULL;                /* 0 for even i, 1 for odd i */
        seq_t binom = (start == 0) ? (seq_t)1 : (seq_t)i;  /* binom(i,start) */

        for (idx_t k = start; k <= i - 1; k += 2) {
            sum += binom * e[k];
            /* advance binom from binom(i,k) to binom(i,k+2) */
            if (k + 2 <= i - 1) {
                seq_t next = binom * (seq_t)(i - k) / (idx_t)(k + 1);
                binom = next * (seq_t)(i - k - 1) / (idx_t)(k + 2);
            }
        }
        e[i] = -sum;
    }

    /* 2. s[n] = e[n] + sin part (only odd n) */
    for (idx_t i = 0; i < N; ++i) {
        seq_t sin_part = 0;
        if (i & 1ULL) { /* odd index */
            idx_t m = (i - 1) >> 1;          /* m = (i-1)/2 */
            sin_part = (m & 1ULL) ? (seq_t)-1 : (seq_t)1;
        }
        s[i] = e[i] + sin_part;
    }

    /* 3. Compute 1 / h(x) coefficients (inverse series) */
    invH[0] = (seq_t)1;
    for (idx_t i = 1; i < N; ++i) {
        seq_t sum   = 0;
        seq_t binom = 1;                     /* binom(i,0) */
        for (idx_t k = 1; k <= i; ++k) {
            /* update binom to binom(i,k) */
            binom = binom * (seq_t)(i - k + 1) / (idx_t)k;
            sum += binom * s[k] * invH[i - k];
        }
        invH[i] = -sum;
    }

    /* 4. Logarithm: compute a[n] via convolution of s and invH */
    a_arr[0] = (seq_t)0;                     /* a(0) = 0 */
    for (idx_t i = 0; i < N - 1; ++i) {      /* i corresponds to n-1 */
        seq_t sum   = 0;
        seq_t binom = 1;                     /* binom(i,0) */
        for (idx_t k = 0; k <= i; ++k) {
            seq_t term = binom * s[k + 1] * invH[i - k];
            sum += term;
            if (k + 1 <= i) {
                /* update binom to binom(i,k+1) */
                binom = binom * (seq_t)(i - k) / (idx_t)(k + 1);
            }
        }
        a_arr[i + 1] = sum;                  /* a(n) for n = i+1 */
    }

    seq_t result = a_arr[n];

    free(e);
    free(s);
    free(invH);
    free(a_arr);

    return result;
}
