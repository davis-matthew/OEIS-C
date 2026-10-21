/*
 * A060820  a(n) = (2*n-1)^2 + (2*n)^2.
 * https://oeis.org/A060820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n-1)^2 + (2*n)^2
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t two_n = nn + nn;                 /* 2*n */
    seq_t two_n_minus1 = two_n - (seq_t)1; /* 2*n - 1 */
    return two_n_minus1 * two_n_minus1 + two_n * two_n;
}
