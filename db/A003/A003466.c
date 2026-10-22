/*
 * A003466  Number of minimal covers of an n-set that have exactly one point
 *          which appears in more than one set in the cover.
 * https://oeis.org/A003466
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{k=1..n-1} (2^k-k-1) * S2(n-1,k), S2 = Stirling
 *            numbers of the second kind
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 3) return 0;
    idx_t m = n - 1;
    seq_t *S = (seq_t *)malloc((size_t)(m + 1) * sizeof(seq_t));
    idx_t i, k;
    seq_t sum, p, res;

    for (i = 0; i <= m; i++) S[i] = 0;
    S[0] = 1;
    for (i = 1; i <= m; i++) {
        for (k = i; k >= 1; k--)
            S[k] = (seq_t)k * S[k] + S[k - 1];
        S[0] = 0;
    }

    sum = 0;
    p = 1;
    for (k = 1; k <= m; k++) {
        p = p * 2;
        sum += (p - (seq_t)k - 1) * S[k];
    }
    res = (seq_t)n * sum;
    free(S);
    return res;
}
