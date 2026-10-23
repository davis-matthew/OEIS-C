/*
 * A040596  Continued fraction for sqrt(621).
 * https://oeis.org/A040596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = 24; a(n) = [1,11,2,11,1,48][(n-1) mod 6] for n>0
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)24;
    static const seq_t cyc[] = { (seq_t)1, (seq_t)11, (seq_t)2,
                                 (seq_t)11, (seq_t)1, (seq_t)48 };
    return cyc[(n - 1) % 6];
}
