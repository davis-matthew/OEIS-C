/*
 * A016827  a(n) = (4n+2)^3.
 * https://oeis.org/A016827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n+2)^3
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 405, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the middle term 4n+2 in seq_t */
    seq_t t = (seq_t)n * 4 + 2;

    /* Return the cube */
    return t * t * t;
}
