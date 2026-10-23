/*
 * A017034  a(n) = (7*n + 4)^6.
 * https://oeis.org/A017034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 4)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 5, u64 n <= 231, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = ((seq_t)7 * (seq_t)n) + 4;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t res = x3 * x3;
    return res;
}
