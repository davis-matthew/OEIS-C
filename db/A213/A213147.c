/*
 * A213147  Polylogarithm li(-n,-5/8) multiplied by (13^(n+1))/8.
 * https://oeis.org/A213147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = li(-n,-5/8) * (13^(n+1))/8
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 13, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) is a special case */
    if (n == 0) return (seq_t)1;

    /* convert to size_t for memory allocation */
    size_t N = (size_t)n;

    /* Allocate required arrays */
    seq_t *S  = (seq_t*)calloc(N + 2, sizeof(seq_t));   /* Stirling numbers S(n+1,k) */
    seq_t *fact = (seq_t*)calloc(N + 1, sizeof(seq_t)); /* factorials */
    seq_t *pow5 = (seq_t*)calloc(N + 2, sizeof(seq_t)); /* powers of 5 */
    seq_t *pow13 = (seq_t*)calloc(N + 1, sizeof(seq_t));/* powers of 13 */

    if (!S || !fact || !pow5 || !pow13) {
        /* Allocation failed – return 0 (should not happen in tests) */
        if (S) free(S);
        if (fact) free(fact);
        if (pow5) free(pow5);
        if (pow13) free(pow13);
        return (seq_t)0;
    }

    /* Compute Stirling numbers S(n+1,k) via 1‑D DP */
    S[1] = (seq_t)1;          /* S(1,1)=1 */
    for (idx_t m = 2; m <= n + 1; ++m) {
        for (idx_t k = m; k >= 1; --k) {
            S[k] = (seq_t)k * S[k] + S[k - 1];
        }
    }

    /* Pre‑compute factorials, powers of 5 and 13 */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    pow5[0] = (seq_t)1;
    for (idx_t i = 1; i <= n + 1; ++i) {
        pow5[i] = pow5[i - 1] * (seq_t)5;
    }

    pow13[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        pow13[i] = pow13[i - 1] * (seq_t)13;
    }

    /* Sum the terms of the closed formula */
    seq_t sum = (seq_t)0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t term = S[j + 1];                 /* S(n+1,j+1) */
        if (j % 2 == 0) term = -term;          /* sign (-1)^(j+1) */
        term *= fact[j];                       /* * j! */
        term *= pow5[j + 1];                   /* * 5^(j+1) */
        term *= pow13[n - j];                  /* * 13^(n-j) */
        sum += term;
    }

    seq_t result = sum / (seq_t)8;             /* exact division by 8 */

    /* Free memory */
    free(S);
    free(fact);
    free(pow5);
    free(pow13);

    return result;
}
