/*
 * A158560  a(n) = 30*n^2 - 1.
 * https://oeis.org/A158560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*n^2 - 1
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn * (seq_t)30;
    val -= (seq_t)1;
    return val;
}
