/*
 * A063239  Duplicate of A063226.
 * https://oeis.org/A063239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 10*floor((n-1)/2) + (n mod 2 ? 3 : 7)
 * Range:     i8 n <= 19, i16 n <= 19, i32 n <= 19, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute k = floor((n-1)/2) */
    idx_t k = (n - 1) / 2;
    /* compute the nth term */
    seq_t result = (seq_t)10 * (seq_t)k + ((n & 1) ? 3 : 7);
    return result;
}
