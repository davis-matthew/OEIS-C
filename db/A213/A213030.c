/*
 * A213030  a(n) = floor(2*n/3)^2 - floor(n/3)^2.
 * https://oeis.org/A213030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*n/3)^2 - floor(n/3)^2
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = (nn * 2) / 3;
    seq_t t2 = nn / 3;
    return t1 * t1 - t2 * t2;
}
