/*
 * A128789  a(n) = n^3*2^n.
 * https://oeis.org/A128789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * 2^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 47, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= 2;

    /* multiply by n^3 */
    seq_t nn = (seq_t)n;
    seq_t res = pow2;
    res *= nn;
    res *= nn;
    res *= nn;

    return res;
}
