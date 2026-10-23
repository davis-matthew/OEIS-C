/*
 * A019432  Continued fraction for tan(1/9).
 * https://oeis.org/A019432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 0, a(1) = 8; for n >= 2: if n is even then a(n) = 1 if n
 *            is odd then a(n) = 9*n - 2
 * Range:     u8 n <= 28, u16 n <= 7282, u32 n <= 9458, u64 n <= 9458,
 *            u128 n <= 9458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 8;
    if ((n & 1) == 0) return 1;
    seq_t sn = (seq_t)n;
    return 9 * sn - 2;
}
