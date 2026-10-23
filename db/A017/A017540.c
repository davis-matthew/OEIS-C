/*
 * A017540  a(n) = (12*n + 1)^8.
 * https://oeis.org/A017540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 1)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 21, u128 n <= 5461,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = ((seq_t)n * 12) + 1;
    seq_t m2 = m * m;
    seq_t m4 = m2 * m2;
    seq_t m8 = m4 * m4;
    return m8;
}
