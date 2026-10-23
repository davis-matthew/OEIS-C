/*
 * A118239  Engel expansion of cosh(1).
 * https://oeis.org/A118239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = 2*(n-1)*(2*n-3) = 4*n^2 - 10*n + 6
 * Range:     u8 n <= 9, u16 n <= 129, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }
    seq_t nn = (seq_t)n;
    seq_t res = 4 * nn * nn - 10 * nn + 6;
    return res;
}
