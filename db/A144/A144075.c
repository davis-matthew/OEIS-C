/*
 * A144075  Duplicate of A008621.
 * https://oeis.org/A144075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = ⌊n/4⌋ + 1
 * Range:     i8 n <= 81, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; 0 is not used in the OEIS data but is handled safely. */
    return (n == 0 ? (seq_t)0 : ((seq_t)n) / 4 + 1);
}
