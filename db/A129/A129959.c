/*
 * A129959  A129957(n) - n*(n-1)/2.
 * https://oeis.org/A129959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1+(-1)^n+n-n^2+2*n^3)/2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    long long nn = (long long)n;
    long long term = 1 + ((n & 1) ? -1 : 1) + nn - nn * nn + 2 * nn * nn * nn;
    return (seq_t)(term / 2);
}
