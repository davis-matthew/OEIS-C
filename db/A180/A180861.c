/*
 * A180861  Wiener index of the n-pan graph.
 * https://oeis.org/A180861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/8)*n*(n^2 + 2*n + 8) if n even; a(n) = (1/8)*(n^3 +
 *            2*n^2 + 7*n - 2) if n odd
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {          /* even n */
        return nn * (nn * nn + 2 * nn + 8) / 8;
    } else {                      /* odd n */
        return (nn * nn * nn + 2 * nn * nn + 7 * nn - 2) / 8;
    }
}
