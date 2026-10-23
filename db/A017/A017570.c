/*
 * A017570  a(n) = (12*n + 4)^2.
 * https://oeis.org/A017570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 4)^2
 * Range:     u8 n <= 0, u16 n <= 20, u32 n <= 5460, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (12*n + 4) first in seq_t to avoid narrowing */
    seq_t t = (seq_t)12 * n + (seq_t)4;
    return t * t;
}
