/*
 * A047267  Numbers that are congruent to {0, 2, 5} mod 6.
 * https://oeis.org/A047267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n-1) - 3*floor((n-1)/3) - ((n-1)^2 mod 3), offset 1
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n - 1;                   /* shift to 0‑based index */
    seq_t term = (seq_t)3 * m;         /* 3m */
    term -= (seq_t)3 * (m / 3);        /* subtract 3 * floor(m/3) */
    seq_t mod3 = ((seq_t)m * (seq_t)m) % 3; /* (m²) mod 3 */
    term -= mod3;
    return term;
}
