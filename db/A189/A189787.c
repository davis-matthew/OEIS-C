/*
 * A189787  a(n) = A189785(n)/2.
 * https://oeis.org/A189787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n - 1 - floor(n/48)
 * Range:     u8 n <= 64, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sub = nn / (seq_t)48;
    return 4 * nn - 1 - sub;
}
