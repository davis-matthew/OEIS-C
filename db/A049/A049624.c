/*
 * A049624  Essentially the same as A032766.
 * https://oeis.org/A049624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = ⌊3*n/2⌋ with a(0) = 1 (offset 0)
 * Range:     i8 n <= 24, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    return ((seq_t)3 * (seq_t)n) / (seq_t)2;
}
