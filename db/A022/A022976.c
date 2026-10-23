/*
 * A022976  a(n) = 20-n.
 * https://oeis.org/A022976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 20 - n
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the unsigned index to the sequence type and subtract. */
    return (seq_t)20 - (seq_t)n;
}
