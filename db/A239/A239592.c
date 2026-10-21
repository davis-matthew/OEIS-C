/*
 * A239592  a(n) = (n^4 - n^3 + 4*n^2 + 2)/2.
 * https://oeis.org/A239592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - n^3 + 4*n^2 + 2)/2
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 293, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t once to avoid repeated casts */
    seq_t sn = (seq_t)n;

    /* Compute powers of n in seq_t to preserve precision */
    seq_t n2 = sn * sn;               /* n^2   */
    seq_t n3 = n2 * sn;               /* n^3   */
    seq_t n4 = n3 * sn;               /* n^4   */

    /* Numerator of the formula */
    seq_t numerator = n4 - n3 + 4 * n2 + 2;

    /* Divide by 2 – the expression is always even */
    return numerator / 2;
}
