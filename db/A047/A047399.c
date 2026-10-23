/*
 * A047399  Numbers that are congruent to {0, 3, 6} mod 8.
 * https://oeis.org/A047399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((8*n-6)/3)
 * Range:     u8 n <= 32, u16 n <= 9893, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The index n is 1‑based and the formula works for all n >= 1. */
    seq_t t = (seq_t)n * 8;  /* 8*n  (no overflow for the intended n range) */
    t = t - 6;              /* 8*n - 6 */
    t = t / 3;              /* floor((8*n-6)/3) */
    return t;
}
