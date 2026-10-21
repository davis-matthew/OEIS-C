/*
 * A210977  A005475 and positive terms of A000566 interleaved.
 * https://oeis.org/A210977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n^2+6*n-1-(2*n-1)*(-1)^n)/16
 * Range:     u8 n <= 20, u16 n <= 323, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent split form:
   if n even: a(n) = (5*n^2 + 2*n) / 8
   else      a(n) = (5*n^2 + 4*n - 1) / 8 */
seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {
        /* n even: (5*n^2 + 2*n) / 8 */
        return (nn * nn * 5 + nn * 2) >> 3;
    } else {
        /* n odd: (5*n^2 + 4*n - 1) / 8 */
        return (nn * nn * 5 + nn * 4 - 1) >> 3;
    }
}
