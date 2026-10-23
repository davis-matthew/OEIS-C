/*
 * A162167  E.g.f. satisfies: A(x) = exp(x*exp(2*x*exp(3*x*A(x)))).
 * https://oeis.org/A162167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Sum_{j=0..k}
 *            2^(k-j)*3^j*C(n,k)*C(k,j)*(j+1)^(n-k-1)*(n-k)^(k-j)*(k-j)^j
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* External linkage function as required */
seq_t a(idx_t n)
{
    /* Allocate coefficient arrays */
    seq_t *A = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *B = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *E = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!A || !B || !E) {
        /* Allocation failed: return 0 (undefined for this sequence) */
        free(A); free(B); free(E);
        return (seq_t)0;
    }

    /* Initial values */
    A[0] = 1;
    B[0] = 1;
    E[0] = 1;

    /* Compute coefficients up to n */
    for (idx_t i = 1; i <= n; ++i) {
        /* ---- Compute B[i] ---- */
        seq_t sumB = 0;
        seq_t binomB = 1;   /* binomial (i-1 choose p) */
        for (idx_t p = 0; p < i; ++p) {
            seq_t h   = (seq_t)3 * (seq_t)(p + 1) * A[p];          /* h_{p+1} = 3*(p+1)*a[p] */
            seq_t temp = h * B[i - 1 - p];
            sumB += binomB * temp;

            if (p + 1 < i) {  /* update binomial for next p */
                binomB = binomB * (seq_t)(i - 1 - p);
                binomB = binomB / (seq_t)(p + 1);
            }
        }
        B[i] = sumB;

        /* ---- Compute E[i] ---- */
        seq_t sumE = 0;
        seq_t binomE = 1;
        for (idx_t p = 0; p < i; ++p) {
            seq_t g   = (seq_t)2 * (seq_t)(p + 1) * B[p];          /* g_{p+1} = 2*(p+1)*B[p] */
            seq_t temp = g * E[i - 1 - p];
            sumE += binomE * temp;

            if (p + 1 < i) {
                binomE = binomE * (seq_t)(i - 1 - p);
                binomE = binomE / (seq_t)(p + 1);
            }
        }
        E[i] = sumE;

        /* ---- Compute A[i] ---- */
        seq_t sumA = 0;
        seq_t binomA = 1;
        for (idx_t p = 0; p < i; ++p) {
            seq_t f   = (seq_t)(p + 1) * E[p];                    /* f_{p+1} = (p+1)*E[p] */
            seq_t temp = f * A[i - 1 - p];
            sumA += binomA * temp;

            if (p + 1 < i) {
                binomA = binomA * (seq_t)(i - 1 - p);
                binomA = binomA / (seq_t)(p + 1);
            }
        }
        A[i] = sumA;
    }

    seq_t result = A[n];
    free(A);
    free(B);
    free(E);
    return result;
}
