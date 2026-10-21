/*
 * A212893  Number of quadruples (w,x,y,z) with all terms in {0,...,n} such
 *          that w-x, x-y, and y-z all have the same parity.
 * https://oeis.org/A212893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + n*(n+1)/2 + floor(n/2))^2
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 361, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t s = 1 + nn * (nn + 1) / 2 + (seq_t)(n / 2);
    return s * s;
}
