/*
 * A007893  A Kutz sequence.
 * https://oeis.org/A007893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 4*floor(n/5))^2
 * Range:     u8 n <= 63, u16 n <= 1263, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t temp = (seq_t)n - 4 * ((seq_t)n / 5);
    return temp * temp;
}
