/*
 * A115293  Row sums of correlation triangle for (1+x)^3/(1-x).
 * https://oeis.org/A115293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..n} binomial(5, n-k) * binomial(k+2, k)
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t C5[6] = {1, 5, 10, 10, 5, 1};
    seq_t result = 0;
    idx_t start = (n >= 5) ? n - 5 : 0;
    for (idx_t k = start; k <= n; ++k) {
        idx_t m = n - k;              /* 0 ≤ m ≤ 5 */
        seq_t coeff = C5[m];
        seq_t kk = (seq_t)k;
        seq_t val = (kk + 2) * (kk + 1) / 2; /* binomial(k+2, k) */
        result += coeff * val;
    }
    return result;
}
