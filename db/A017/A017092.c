/*
 * A017092  a(n) = (8*n + 2)^4.
 * https://oeis.org/A017092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 2)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 31, u64 n <= 8191,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)8 * n + 2;   /* compute 8*n + 2 in seq_t */
    seq_t s = t * t;              /* square once: (8*n + 2)^2 */
    return s * s;                 /* square again: (8*n + 2)^4 */
}
