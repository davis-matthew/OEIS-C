/*
 * A022981  a(n) = 25-n.
 * https://oeis.org/A022981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 25 - n
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the integer index to seq_t before subtraction to avoid
       any implicit narrowing or overflow on the intermediate result. */
    return (seq_t)25 - (seq_t)n;
}
