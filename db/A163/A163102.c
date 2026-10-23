/*
 * A163102  a(n) = n^2*(n+1)^2/2.
 * https://oeis.org/A163102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n^2 * (n+1)^2 / 2
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 271, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n1 = nn + 1;

    /* t = n * (n + 1) is always even, so t * t is divisible by 4 */
    seq_t t = nn * n1;
    seq_t res = t * t;
    res /= 2;
    return res;
}
