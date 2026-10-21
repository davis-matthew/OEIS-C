/*
 * A228564  Largest odd divisor of n^2 + 1.
 * https://oeis.org/A228564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd part of n² + 1 (largest odd divisor of n² + 1)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * (seq_t)n + 1;   /* n² + 1 */
    while ((x % 2) == 0) {
        x /= 2;                         /* strip powers of 2 */
    }
    return x;                            /* largest odd divisor */
}
