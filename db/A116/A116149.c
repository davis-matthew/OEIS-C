/*
 * A116149  a(n) = sum of n consecutive cubes after n^3.
 * https://oeis.org/A116149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(1 + 3*n)*(3 + 5*n)/4
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 182, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t term1 = N * N;          /* n^2 */
    seq_t term2 = N * 3 + 1;      /* 1 + 3n */
    seq_t term3 = N * 5 + 3;      /* 3 + 5n */
    seq_t res = term1 * term2;
    res *= term3;
    res /= 4;
    return res;
}
