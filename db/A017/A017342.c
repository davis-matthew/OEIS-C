/*
 * A017342  a(n) = (10*n + 6)^2.
 * https://oeis.org/A017342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 6)^2
 * Range:     u8 n <= 0, u16 n <= 24, u32 n <= 6552, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use seq_t arithmetic only; cast n to seq_t before multiplication */
    seq_t t = (seq_t)10 * (seq_t)n + (seq_t)6;
    return t * t;
}
