/*
 * A195145  Concentric 14-gonal numbers.
 * https://oeis.org/A195145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n^2 + 5*(-1)^n - 5)/4
 * Range:     u8 n <= 4, u16 n <= 135, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t res = (seq_t)14 * N * N;
    if (n & 1)  /* odd n: -10 before division */
        res -= (seq_t)10;
    res /= 4;
    return res;
}
