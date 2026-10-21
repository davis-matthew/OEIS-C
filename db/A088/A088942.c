/*
 * A088942  a(n)=60*sum(1<=i<=j<=k<=n,i^2*j/k).
 * https://oeis.org/A088942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60*C(n,1) + 270*C(n,2) + 460*C(n,3) + 345*C(n,4) +
 *            96*C(n,5)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t diffs[6] = {0, 60, 270, 460, 345, 96};
    seq_t result = 0;
    for (idx_t k = 1; k <= 5; ++k) {
        if (n < k) break;                /* C(n,k) = 0 when n < k */
        seq_t binom = 1;
        for (idx_t i = 0; i < k; ++i) {
            binom = binom * (seq_t)(n - i) / (seq_t)(i + 1);
        }
        result += diffs[k] * binom;
    }
    return result;
}
