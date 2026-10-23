/*
 * A137936  a(n) = 5*mod(n,5) + floor(n/5).
 * https://oeis.org/A137936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*mod(n,5) + floor(n/5)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t q = sn / 5;
    seq_t r = sn % 5;
    return 5 * r + q;
}
