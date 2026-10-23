/*
 * A140672  a(n) = n*(3*n + 13)/2.
 * https://oeis.org/A140672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(3*n + 13)/2
 * Range:     u8 n <= 11, u16 n <= 206, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result = nn * ((seq_t)3 * nn + 13) / 2;
    return result;
}
