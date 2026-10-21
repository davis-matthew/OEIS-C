/*
 * A237991  a(n) = 991*n^2 + 1.
 * https://oeis.org/A237991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 991*n^2 + 1
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t ns2 = ns * ns;
    seq_t res = ns2 * (seq_t)991 + (seq_t)1;
    return res;
}
