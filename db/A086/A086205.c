/*
 * A086205  Determinant of n X n matrix M_(i,j) = binomial(i^2, j).
 * https://oeis.org/A086205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=1..n} (2*k-1)!/(k-1)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        /* product from i = k to 2*k-1 gives (2k-1)!/(k-1)! */
        for (idx_t i = k; i <= (2 * k) - 1; ++i) {
            res *= (seq_t)i;
        }
    }
    return res;
}
