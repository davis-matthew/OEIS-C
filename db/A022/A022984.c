/*
 * A022984  a(n) = 28-n.
 * https://oeis.org/A022984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 28 - n
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the term by a simple linear expression. */
    /* cast to seq_t to avoid unsigned wrap‑around */
    return (seq_t)28 - (seq_t)n;
}
