/*
 * A053106  a(n) = ((7*n+10)(!^7))/10(1^7), related to A034830
 *          (((7*n+3)(!^7))/3 sept-, or 7-factorials).
 * https://oeis.org/A053106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k=0}^{n-1} (7k + 17) (with a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = 1;
    for (idx_t k = 0; k < n; ++k) {
        res *= (seq_t)(7 * k + 17);
    }
    return res;
}
