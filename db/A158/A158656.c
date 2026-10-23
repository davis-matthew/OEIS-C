/*
 * A158656  a(n) = 54*n^2 - 1.
 * https://oeis.org/A158656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 54*n^2 - 1
 * Range:     u8 n <= 2, u16 n <= 34, u32 n <= 8918, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t res = N * N;
    res *= 54;
    res -= 1;
    return res;
}
