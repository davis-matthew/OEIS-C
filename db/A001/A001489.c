/*
 * A001489  a(n) = -n.
 * https://oeis.org/A001489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  core, sign, easy
 * Formula:   a(n) = -n
 * Range:     i8 n <= 128, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	return -(seq_t)n;
}
