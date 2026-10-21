/*
 * A228702  Wiener index of generalized Fibonacci cube graph Q_n(111).
 * https://oeis.org/A228702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1..n} S[i-1] · S[n-i] · ( E[i-1] · (E[n-i] + D[n-i])
 *            + D[i-1] · E[n-i] ) where: S[k] – number of binary strings of
 *            length k that avoid "111" (tribonacci numbers with S[0]=1,
 *            S[1]=2, S[2]=4, S[k]=S[k-1]+S[k-2]+S[k-3] for k≥3) E[k] – number
 *            of such strings that end with 0 (E[0]=1, E[k]=...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Allocate space for tribonacci values and helper counts */
    size_t N = (size_t)n + 1;
    seq_t *S   = (seq_t *)calloc(N, sizeof(seq_t));   /* S[0..n] */
    seq_t *E   = (seq_t *)calloc(N, sizeof(seq_t));   /* E[0..n] = end-with-0  */
    seq_t *D   = (seq_t *)calloc(N, sizeof(seq_t));   /* D[0..n] = end-with-single-1 */

    if (!S || !E || !D) { return 0; } /* memory failure – return 0 as a safe fallback */

    /* Tribonacci sequence S[k] = #strings of length k avoiding "111" */
    S[0] = 1;
    if (n >= 1) S[1] = 2;
    if (n >= 2) S[2] = 4;
    for (size_t k = 3; k <= n; ++k) {
        S[k] = S[k-1] + S[k-2] + S[k-3];
    }

    /* Helper counts */
    E[0] = 1;                      /* empty string ends with 0 by convention */
    if (n >= 1) E[1] = S[0];       /* last bit 0: only "0" */
    for (size_t k = 2; k <= n; ++k) E[k] = S[k-1];

    D[0] = 0;
    if (n >= 1) D[1] = 1;          /* string "1" */
    for (size_t k = 2; k <= n; ++k) D[k] = S[k-2];

    /* Compute the Wiener index */
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        size_t L = (size_t)(i-1);      /* length of left part */
        size_t R = (size_t)(n-i);      /* length of right part */

        seq_t sz0 = S[L] * S[R];

        seq_t sz1 = E[L] * (E[R] + D[R]) + D[L] * E[R];

        result += sz0 * sz1;
    }

    /* Clean up */
    free(S);
    free(E);
    free(D);

    return result;
}
