/*
 * A000559  Generalized Stirling numbers of second kind.
 * https://oeis.org/A000559
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} Stirling2(n,k) * Stirling2(k,3), where
 *            Stirling2(k,3) = 3*Stirling2(k-1,3) + Stirling2(k-1,2),
 *            Stirling2(k,2) = 2*Stirling2(k-1,2) + Stirling2(k-1,1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 3) return 0;

    seq_t *S = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= n; i++) S[i] = 0;
    S[0] = 1;

    /* Build row n of Stirling2 numbers in place: S[j] = Stirling2(i, j) */
    for (idx_t i = 1; i <= n; i++) {
        for (idx_t j = i; j >= 1; j--) {
            S[j] = S[j - 1] + (seq_t)j * S[j];
        }
        S[0] = 0;
    }

    seq_t s1 = 0, s2 = 0, s3 = 0; /* Stirling2(k,1), Stirling2(k,2), Stirling2(k,3) at k = 0 */
    seq_t result = 0;
    for (idx_t k = 1; k <= n; k++) {
        seq_t ns3 = s3 * 3 + s2;
        seq_t ns2 = s2 * 2 + s1;
        s3 = ns3;
        s2 = ns2;
        s1 = 1;
        if (k >= 3) result += S[k] * s3;
    }

    free(S);
    return result;
}
