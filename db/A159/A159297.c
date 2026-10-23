/*
 * A159297  Number of 3D matrices with positive integer entries such that sum
 *          of all entries equals n.
 * https://oeis.org/A159297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i,j,k>0, i*j*k <= n} C(n-1, i*j*k - 1)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t N = n;
    idx_t N1 = N - 1;

    /* Precompute binomial coefficients C(N-1, r) for r = 0 .. N-1 */
    seq_t *binom = (seq_t *)malloc(N * sizeof(seq_t));
    if (!binom)
        return 0;   /* Out of memory – caller should handle */

    binom[0] = 1;
    for (idx_t r = 0; r < N1; ++r) {
        seq_t num = binom[r] * (seq_t)(N1 - r);
        seq_t den = (seq_t)(r + 1);
        binom[r + 1] = num / den;   /* Exact division */
    }

    seq_t sum = 0;
    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = 1; j <= N / i; ++j) {
            idx_t ij = i * j;
            for (idx_t k = 1; k <= N / ij; ++k) {
                idx_t m = ij * k;          /* product i*j*k */
                sum += binom[m - 1];       /* add C(N-1, m-1) */
            }
        }
    }

    free(binom);
    return sum;
}
