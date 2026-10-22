/*
 * A002629  Number of permutations of length n with one 3-sequence.
 * https://oeis.org/A002629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum(binomial(n-k-2,k-1)*A000166(n-k),
 *            k=1..floor((n-1)/2)), A000166(m) = derangements: D(0)=1, D(1)=0,
 *            D(m) = (m-1)*(D(m-1)+D(m-2))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n <= 2) return 0;

    seq_t *D = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    D[0] = 1;
    D[1] = 0;
    for (idx_t i = 2; i < n; i++)
        D[i] = (seq_t)(i - 1) * (D[i - 1] + D[i - 2]);

    seq_t result = 0;
    idx_t max_k = (n - 1) / 2;
    seq_t B = 1; /* binomial(n-k-2, k-1) for k = 1 */

    for (idx_t k = 1; k <= max_k; k++) {
        result += B * D[n - k];
        if (k < max_k) {
            /* B(k+1) = B(k) * (n-2k-1)(n-2k-2) / (k*(n-k-2)) */
            seq_t num = (seq_t)(n - 2 * k - 1) * (seq_t)(n - 2 * k - 2);
            seq_t den = (seq_t)k * (seq_t)(n - k - 2);
            if (den == 0) {
                B = 0; /* only in a narrow fixed-width build, where the result overflows anyway */
            } else {
                B = B * num / den;
            }
        }
    }

    free(D);
    return result;
}
