/*
 * A047348  Numbers that are congruent to {2, 4} mod 7.
 * https://oeis.org/A047348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/2) + 3 + (-1)^n
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent implementation avoiding negative values:          */
/* a(n) = 7*floor((n-1)/2) + (n odd ? 2 : 4)                      */
seq_t a(idx_t n)
{
    seq_t k   = ((seq_t)n - 1) / 2;   /* floor((n-1)/2) */
    seq_t base = k * 7;
    return base + ((n & 1) ? (seq_t)2 : (seq_t)4);
}
