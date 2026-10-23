/*
 * A047310  Numbers that are congruent to {0, 1, 3, 4, 5, 6} mod 7.
 * https://oeis.org/A047310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ((n+3)/6) - 1
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (derived from a(n) = n + floor((n-3)/6) for n ≥ 1) */
seq_t a(idx_t n)
{
    return (seq_t)n + ((seq_t)n + 3) / 6 - 1;
}
