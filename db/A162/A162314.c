/*
 * A162314  Row sums of A162313.
 * https://oeis.org/A162314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; for n > 0 a(n) = 2^n + Σ_{k=0}^{n-1} C(n,k) * 2^{n-k}
 *            * a(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */

#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n) {
    /* allocate arrays for a[0..n] and powers of two 2^i */
    seq_t *A = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *P = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!A || !P) return 0;   /* allocation failed */

    /* precompute powers of two */
    P[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        P[i] = P[i - 1] * ((seq_t)2);

    /* base case */
    A[0] = (seq_t)1;

    /* recurrence */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;                 /* Σ binom(i,k) * 2^{i-k} * A[k] for k=0..i-1 */
        seq_t binom = (seq_t)1;        /* binom(i,0) */

        for (idx_t k = 0; k < i; ++k) {
            seq_t temp = binom * P[i - k];   /* 2^{i-k} * binom(i,k) */
            temp        = temp * A[k];        /* * a(k) */
            sum         = sum + temp;

            /* update binom to binom(i,k+1) for next iteration */
            if (k + 1 < i) {
                binom = binom * ((seq_t)(i - k));   /* multiply by (i-k) */
                binom = binom / ((seq_t)(k + 1));   /* divide by (k+1) */
            }
        }

        A[i] = P[i] + sum;   /* a(i) = 2^i + sum */
    }

    seq_t result = A[n];
    free(A);
    free(P);
    return result;
}
