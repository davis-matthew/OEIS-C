/*
 * A074227  Duplicate of A042965.
 * https://oeis.org/A074227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n + floor((n + 1)/3)
 * Range:     i8 n <= 25, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Add n to the count of excluded numbers.
       The excluded numbers are those congruent to 2 (mod 4).
       In every block of 4 integers, one number is omitted,
       so floor((n + 1)/3) gives that count for the first n terms. */
    return (seq_t)n + (seq_t)((n + 1) / 3);
}
