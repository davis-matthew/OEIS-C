/*
 * A022983  a(n) = 27 - n.
 * https://oeis.org/A022983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 27 - n
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the unsigned index to the sequence integer type and
       subtract from 27.  For n where the result clearly overflows
       seq_t this will wrap as per the C implementation, which is
       acceptable since the specification only requires correctness
       when the value fits in seq_t. */
    seq_t result = (seq_t)27;
    result -= (seq_t)n;
    return result;
}
