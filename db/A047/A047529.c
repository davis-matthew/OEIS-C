/*
 * A047529  Numbers that are congruent to {1, 3, 7} mod 8.
 * https://oeis.org/A047529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n + floor(n/3)) - 1 (for n >= 1)
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that are congruent to {1,3,7} mod 8 */

seq_t a(idx_t n)
{
    /* Ensure n is at least 1; for n == 0 the result is undefined but harmless */
    if (n == 0) return 0;

    /* Compute a(n) using 64‑bit arithmetic safely promoted to seq_t */
    seq_t term = (seq_t)n + (seq_t)(n / 3);   /* n + floor(n/3) */
    term = term * 2 - 1;                     /* 2*(...)-1 */

    return term;
}
