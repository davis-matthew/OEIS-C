/*
 * A168213  a(n) = (11 + 18*n + 9*(-1)^n)/4.
 * https://oeis.org/A168213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11 + 18*n + 9*(-1)^n)/4 Computation: a(n) = (9*n +
 *            (n%2?1:10))/2 Works for any unsigned index n >= 1
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute numerator 9*n + (n%2?1:10) using idx_t (64‑bit) */
    idx_t numerator = (idx_t)9 * n + ((n & 1) ? (idx_t)1 : (idx_t)10);
    /* Divide by 2 (exact division) */
    idx_t value = numerator / 2;
    return (seq_t)value;
}
