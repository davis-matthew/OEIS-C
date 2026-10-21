/*
 * A060798  Numbers k such that difference between the upper and lower central
 *          divisors of k is 1.
 * https://oeis.org/A060798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1) for n > 2, a(1) = 2, a(2) = 4
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    return (seq_t)n * ((seq_t)n - 1);
}
