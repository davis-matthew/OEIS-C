/*
 * A077051  Right summatory matrix, T, by antidiagonals.
 * https://oeis.org/A077051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if (d-(n-T_{d-1})+1) divides (n-T_{d-1}); otherwise 0.
 *            Here T_k = k*(k+1)/2 and d is the smallest integer with T_d >= n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find diagonal number d such that T_{d-1} < n <= T_d */
    idx_t d   = 1;
    idx_t tri = 1;          /* tri = T_d = d*(d+1)/2 */
    while (tri < n) {
        ++d;
        tri += d;
    }

    /* Position within the diagonal: 1 .. d */
    idx_t pos = n - (tri - d);

    /* Row index i = d - pos + 1 */
    idx_t i = d - pos + 1;

    /* a(n) = 1 iff i divides pos */
    return ((pos % i) == 0) ? (seq_t)1 : (seq_t)0;
}
