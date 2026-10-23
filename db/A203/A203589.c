/*
 * A203589  Vandermonde sequence using x^2 + y^2 applied to (1,3,5,...,2n-1).
 * https://oeis.org/A203589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{1<=i<j<=n} ((2*i-1)^2 + (2*j-1)^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i < n; ++i) {
        seq_t ai = (seq_t)(2 * i - 1);
        seq_t ai_sq = ai * ai;
        for (idx_t j = i + 1; j <= n; ++j) {
            seq_t bj = (seq_t)(2 * j - 1);
            seq_t term = ai_sq + bj * bj;
            result *= term;
        }
    }
    return result;
}
