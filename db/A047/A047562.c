/*
 * A047562  Numbers that are congruent to {3, 4, 5, 6, 7} mod 8.
 * https://oeis.org/A047562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*((n-1)/5) + ((n-1)%5) + 3 (a(1)=3, a(2)=4, …)
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, not used for n>=1 */
    idx_t m = n - 1;
    idx_t k = m / 5;               /* block index */
    idx_t r = m % 5;               /* position inside a block */
    return (seq_t)8 * (seq_t)k + (seq_t)r + (seq_t)3;
}
