/*
 * A047798  a(n) = Sum_{k=0..n} C(n,k)*Stirling2(n,k)^2.
 * https://oeis.org/A047798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * Stirling2(n,k)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) using a linear recurrence for Stirling numbers of the
   second kind and an iterative computation of the binomial coefficients.
   Works for all n >= 0 and for arbitrary‑precision seq_t. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate space for one row of Stirling numbers: size n+1. */
    seq_t *S = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!S)
        return (seq_t)0;   /* allocation failure – unlikely in test harness */

    /* Initialize the first row: S(0,0) = 1, others 0. */
    for (idx_t i = 0; i <= n; ++i)
        S[i] = 0;
    S[0] = 1;

    seq_t result = 0;

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute Stirling numbers S(i,k) in place for k = i..1.
           S(k) before update holds S(i-1,k); S(k-1) holds S(i-1,k-1). */
        for (idx_t k = i; k >= 1; --k) {
            /* S(i,k) = k * S(i-1,k) + S(i-1,k-1) */
            S[k] = S[k] * (seq_t)k + S[k - 1];
        }
        /* After computing the row, S[0] should be 0 for all n > 0. */
        S[0] = 0;

        /* Accumulate a(i) = sum_{k=0..i} C(i,k) * S[i][k]^2 */
        seq_t sum = 0;
        seq_t bin = 1;   /* C(i,0) = 1 */

        for (idx_t k = 0; k <= i; ++k) {
            seq_t sq   = S[k] * S[k];
            sum        = sum + bin * sq;

            if (k < i) {
                /* Update binomial coefficient iteratively:
                   C(i,k+1) = C(i,k) * (i-k) / (k+1) */
                bin = bin * (seq_t)(i - k) / (seq_t)(k + 1);
            }
        }

        if (i == n) {
            result = sum;
            break;
        }
    }

    free(S);
    return result;
}
