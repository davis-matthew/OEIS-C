/*
 * A092534  Expansion of (1-x+x^2)*(1+x^4)/((1-x)^2*(1-x^2)).
 * https://oeis.org/A092534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0,1 * a(2) = 3 * a(n) = (2*n^2 - 4*n + 13 +
 *            (n%2? -3 : +3)) / 4 for n > 2
 * Range:     u8 n <= 11, u16 n <= 265, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)3;

    seq_t N  = (seq_t)n;
    seq_t nn = N * N;                          /* n^2 */
    seq_t val = (seq_t)2 * nn - (seq_t)4 * N + (seq_t)13;
    if (n & (idx_t)1)
        val -= (seq_t)3;
    else
        val += (seq_t)3;

    return val / (seq_t)4;
}
