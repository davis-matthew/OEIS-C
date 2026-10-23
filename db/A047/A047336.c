/*
 * A047336  Numbers that are congruent to {1, 6} mod 7.
 * https://oeis.org/A047336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/2) + (n%2?1:6)
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 for the OEIS offset 1 */
    idx_t m = (n - 1) / 2;            /* floor((n-1)/2) */
    seq_t base = (seq_t)7 * (seq_t)m; /* 7 * m  (seq_t ensures no overflow in 64‑bit builds) */
    return base + ((n & 1) ? 1 : 6);
}
