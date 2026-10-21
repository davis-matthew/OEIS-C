/*
 * A215543  Number of standard Young tableaux of shape [3n,3].
 * https://oeis.org/A215543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n-2)*(3*n+2)*(n+1)/2 for n>0, a(0)=0
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 810, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t t   = (seq_t)n;
    seq_t a1  = 3 * t - 2;
    seq_t a2  = 3 * t + 2;
    seq_t a3  = t + 1;
    seq_t prod = a1 * a2;
    prod = prod * a3;
    return prod / 2;
}
