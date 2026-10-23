/*
 * A152950  a(n) = 3 + n*(n-1)/2.
 * https://oeis.org/A152950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + n*(n-1)/2
 * Range:     u8 n <= 16, u16 n <= 271, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t = t * (t - 1);
    t /= 2;
    t += (seq_t)3;
    return t;
}
