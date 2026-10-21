/*
 * A097063  Expansion of g.f. (1-2*x+3*x^2)/((1+x)*(1-x)^3).
 * https://oeis.org/A097063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n even) ? n^2/2 + 1 : (n^2 - 1)/2
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

extern seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;              /* cast the index to the sequence type */
    if ((n & 1) == 0) {               /* n is even */
        return nn * nn / 2 + 1;
    } else {                          /* n is odd  */
        return (nn * nn - 1) / 2;
    }
}
