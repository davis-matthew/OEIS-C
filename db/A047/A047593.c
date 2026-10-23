/*
 * A047593  Numbers that are congruent to {2, 3, 4, 5, 6, 7} mod 8.
 * https://oeis.org/A047593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*((n-1)/6)+((n-1)%6)+2
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = (n - 1) / 6;
    idx_t r = (n - 1) % 6;
    return (seq_t)8 * (seq_t)k + (seq_t)(r + 2);
}
