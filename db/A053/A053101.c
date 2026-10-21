/*
 * A053101  a(n) = ((6*n+8)(!^6))/8(!^6), related to A034689 (((6*n+2)(!^6))/2
 *          sextic, or 6-factorials).
 * https://oeis.org/A053101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((6*n+8)(!^6))/8(!^6) = Π_{k=1}^{n} (6k+8)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k) {
        res *= (seq_t)(6 * k + 8);
    }
    return res;
}
