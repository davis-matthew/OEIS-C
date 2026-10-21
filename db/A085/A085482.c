/*
 * A085482  Product of three solutions of the Diophantine equation x^2 - y^2 =
 *          z^3.
 * https://oeis.org/A085482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 54*n^8
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;   /* convert index to sequence type */
    seq_t p2 = x * x;      /* n^2      */
    seq_t p4 = p2 * p2;    /* n^4      */
    seq_t p8 = p4 * p4;    /* n^8      */
    return (seq_t)54 * p8; /* 54 * n^8 */
}
