/*
 * A158633  a(n) = 529*n^2 - 23.
 * https://oeis.org/A158633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 529*n^2 - 23
 * Range:     u8 none, u16 n <= 11, u32 n <= 2849, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* ensure n is treated as seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t t = 529 * nn;
    seq_t res = t * nn;
    res -= 23;
    return res;
}
