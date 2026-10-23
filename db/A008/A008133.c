/*
 * A008133  a(n) = floor(n/3)*floor((n+1)/3).
 * https://oeis.org/A008133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/3) * floor((n+1)/3)
 * Range:     u8 n <= 47, u16 n <= 767, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q1 = (seq_t)n / 3;
    seq_t q2 = ((seq_t)n + 1) / 3;
    return q1 * q2;
}
