/*
 * A063226  Dimension of the space of weight 2n cuspidal newforms for
 *          Gamma_0(63).
 * https://oeis.org/A063226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n - 3 if n even, a(n) = 5*n - 2 if n odd
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)5 * (seq_t)n;
    if ((n & 1) == 0)
        res -= (seq_t)3;
    else
        res -= (seq_t)2;
    return res;
}
