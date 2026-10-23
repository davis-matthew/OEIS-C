/*
 * A145995  a(n) = 8 - 12*n + 5*n^2.
 * https://oeis.org/A145995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 - 12*n + 5*n^2
 * Range:     u8 n <= 8, u16 n <= 115, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return 5 * tn * tn - 12 * tn + 8;
}
