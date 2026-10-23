/*
 * A171108  a(n) is the Severi degree for curves of degree n and cogenus 2.
 * https://oeis.org/A171108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n-1)*(n-2)*(3*n^2-3*n-11)/2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 175, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t a1 = nn - 1;
    seq_t b1 = nn - 2;

    seq_t c = 3 * nn * nn;   /* 3*n^2 */
    c -= 3 * nn;             /* -3*n */
    c -= 11;                 /* -11 */

    seq_t res = a1;
    res *= b1;
    res *= c;
    res *= 3;
    res /= 2;
    return res;
}
