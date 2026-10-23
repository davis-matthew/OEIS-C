/*
 * A047346  Numbers that are congruent to {1, 4} mod 7.
 * https://oeis.org/A047346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7*n-5)/2)
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn   = (seq_t)n;          /* promote index to seq_t */
    seq_t term = (tn * (seq_t)7 - (seq_t)5) / (seq_t)2;
    return term;
}
