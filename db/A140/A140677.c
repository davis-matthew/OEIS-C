/*
 * A140677  a(n) = n*(3*n + 8).
 * https://oeis.org/A140677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(3*n + 8)
 * Range:     u8 n <= 7, u16 n <= 146, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * ((seq_t)n * (seq_t)3 + (seq_t)8);
}
