/*
 * A008627  Molien series for A_4.
 * https://oeis.org/A008627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 6*n^2 + 36*n + 72 - 9*(n mod 2)*n) / 72
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n;
    seq_t n2 = sq * sq;
    seq_t n3 = n2 * sq;
    seq_t numerator = n3 + (seq_t)6 * n2 + (seq_t)36 * sq + (seq_t)72;

    /* subtract 9*n when n is odd */
    seq_t one_bit = sq & (seq_t)1;
    if (one_bit)
        numerator -= (seq_t)9 * one_bit * sq;

    return numerator / (seq_t)72;
}
