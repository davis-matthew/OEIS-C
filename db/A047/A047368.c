/*
 * A047368  Numbers that are congruent to {0, 1, 2, 3, 4, 5} mod 7;
 *          a(n)=floor(7(n-1)/6).
 * https://oeis.org/A047368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(7*(n-1)/6)
 * Range:     u8 n <= 220, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t m = n - 1;
    return (seq_t)((m / 6) * 7u + ((m % 6) * 7u) / 6);
}
