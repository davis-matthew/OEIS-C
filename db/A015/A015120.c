/*
 * A015120  Inverse of 1111th cyclotomic polynomial.
 * https://oeis.org/A015120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if n % 1111 ≤ 10, otherwise 0   (period 1111) */
seq_t a(idx_t n)
{
    const idx_t PERIOD = 1111;
    idx_t r = n % PERIOD;
    return (r <= 10) ? (seq_t)1 : (seq_t)0;
}
