/*
 * A051689  a(n) = (5*n+8)(!^5)/8(!^5), related to A034300 ((5*n+3)(!^5)
 *          quintic, or 5-factorials).
 * https://oeis.org/A051689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Π_{k=0}^{n-1} (5k + 13) (empty product = 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 0; k < n; ++k) {
        res *= (seq_t)5 * k + 13;
    }
    return res;
}
