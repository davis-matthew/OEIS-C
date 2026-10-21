/*
 * A070643  Duplicate of A070442.
 * https://oeis.org/A070643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (n^2) mod 20
 * Range:     i8 n <= 31, i16 n <= 31, i32 n <= 31, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t r = n % 10;          /* reduce modulo 10, same as modulo 20 for squares */
    seq_t rr = (seq_t)r;       /* promote to seq_t */
    return (rr * rr) % (seq_t)20;
}
