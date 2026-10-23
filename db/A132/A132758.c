/*
 * A132758  a(n) = n*(n + 31)/2.
 * https://oeis.org/A132758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+31)/2
 * Range:     u8 n <= 11, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n * (((seq_t)n) + 31)) / 2;
}
