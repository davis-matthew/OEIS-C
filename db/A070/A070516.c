/*
 * A070516  Duplicate of A070435.
 * https://oeis.org/A070516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n^2 mod 12
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 12;          /* reduce n modulo 12 */
    seq_t prod = (seq_t)r * (seq_t)r; /* compute r^2 in seq_t */
    return prod % 12;          /* take mod 12 again */
}
