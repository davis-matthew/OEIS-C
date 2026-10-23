/*
 * A016798  a(n) = (3*n + 2)^10.
 * https://oeis.org/A016798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + 2)^10
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 27, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute t = 3*n + 2 */
    seq_t t = ((seq_t)n * 3) + 2;

    /* compute t^10 by repeated multiplication */
    seq_t res = 1;
    for (int i = 0; i < 10; ++i)
        res *= t;

    return res;
}
