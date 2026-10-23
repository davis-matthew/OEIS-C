/*
 * A134175  a(n) = (32/2)*(n-1)*(n-2)*(n-3)*(n-4).
 * https://oeis.org/A134175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*(n-1)*(n-2)*(n-3)*(n-4)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 130, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return 0;
    seq_t nn = (seq_t)n;
    return (seq_t)16 * (nn - 1) * (nn - 2) * (nn - 3) * (nn - 4);
}
