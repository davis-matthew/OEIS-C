/*
 * A092907  Duplicate of A051022.
 * https://oeis.org/A092907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 100*⌊n/10⌋ + (n mod 10)
 * Range:     i8 n <= 19, i16 n <= 21, i32 n <= 21, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q = (seq_t)n / 10;   /* floor(n/10) */
    seq_t r = (seq_t)n % 10;   /* last decimal digit */
    return q * 100 + r;
}
