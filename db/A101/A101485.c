/*
 * A101485  a(n) = (4n)! / ( 4^n * (2n)! ).
 * https://oeis.org/A101485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n)!/(4^n*(2n)!) = (4n-1)*(4n-3)*a(n-1), a(0)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t factor = (seq_t)(4 * k - 1) * (seq_t)(4 * k - 3);
        res *= factor;
    }
    return res;
}
