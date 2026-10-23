/*
 * A025777  Expansion of 1/((1-x)*(1-x^5)*(1-x^7)).
 * https://oeis.org/A025777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((n^2 + 13*n + 36)/70)
 * Range:     u8 n <= 8, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use integer rounding: floor((x + 35) / 70) */
    seq_t val = (seq_t)n * ((seq_t)n + (seq_t)13) + (seq_t)71; /* n^2 + 13*n + 71 */
    return val / (seq_t)70;
}
