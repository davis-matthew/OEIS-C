/*
 * A076938  Duplicate of A004526.
 * https://oeis.org/A076938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = floor(n/2)
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type and divide by 2.
       Since idx_t is unsigned, the conversion preserves the value. */
    return (seq_t)n / 2;
}
