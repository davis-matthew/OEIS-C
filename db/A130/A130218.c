/*
 * A130218  Partial sums of A100119. Sum of first n of the n-th centered
 *          n-gonal numbers.
 * https://oeis.org/A130218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n*(26-3*n-2*n^2+3*n^3))/24
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute the polynomial 3*n^3 - 2*n^2 - 3*n + 26 */
    seq_t poly = 3 * nn * nn * nn - 2 * nn * nn - 3 * nn + 26;
    return nn * poly / 24;
}
