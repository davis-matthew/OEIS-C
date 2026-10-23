/*
 * A106231  Least j > 1 such that j^2 = (4*n^2 + 2)*(k^2) + (4*n^2 + 2)*k + 1.
 * https://oeis.org/A106231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 - 4*n^2 + 2*n - 1 for n > 1, a(1) = 11
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 11;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    return 4 * n3 - 4 * n2 + 2 * nn - 1;
}
