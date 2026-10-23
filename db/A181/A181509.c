/*
 * A181509  a(1) = 2, a(n) = (n-th-even n^3) - (sum of previous terms).
 * https://oeis.org/A181509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=2, a(2)=6; for n>2, a(n)=24*n^2 - 72*n + 56
 * Range:     u8 n <= 4, u16 n <= 53, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 2;
    if (n == 2) return 6;
    seq_t s = (seq_t)n;
    return s * s * 24 - s * 72 + 56;
}
