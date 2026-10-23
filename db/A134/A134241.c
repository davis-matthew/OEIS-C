/*
 * A134241  a(n) = 8*(n-1)*(n-2)*(n-3)*(6*n^2-37*n+60).
 * https://oeis.org/A134241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(n-1)*(n-2)*(n-3)*(6*n^2-37*n+60)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t term = 6 * nn * nn - 37 * nn + 60;
    seq_t result = 8 * (nn - 1) * (nn - 2) * (nn - 3) * term;
    return result;
}
