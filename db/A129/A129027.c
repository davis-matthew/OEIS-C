/*
 * A129027  Odd-indexed terms of A129026.
 * https://oeis.org/A129027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n*(5+13*n+7*n^2+2*n^3))
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = 5
                 + 13 * nn
                 + 7 * nn * nn
                 + 2 * nn * nn * nn;
    return 4 * nn * term;
}
