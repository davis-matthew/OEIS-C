/*
 * A016791  a(n) = (3*n + 2)^3.
 * https://oeis.org/A016791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + 2)^3
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 541, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t, then compute (3*n + 2) */
    seq_t t = (seq_t)n;
    t = t * (seq_t)3 + (seq_t)2;
    /* Return the cube of the result */
    return t * t * t;
}
