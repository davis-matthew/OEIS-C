/*
 * A000012  The simplest sequence of positive numbers: the all 1's sequence.
 * https://oeis.org/A000012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, mult, cofr, cons, tabl
 * Formula:   a(n) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    (void)n;
    return (seq_t)1;
}
