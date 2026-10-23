/*
 * A047453  Numbers that are congruent to {0, 1, 2, 3, 4} mod 8.
 * https://oeis.org/A047453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + 3 * floor((n - 1)/5)
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is >= 1, so n-1 is safe */
    seq_t m   = (seq_t)(n - 1);
    seq_t div = (seq_t)((n - 1) / 5);
    return m + (seq_t)3 * div;
}
