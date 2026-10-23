/*
 * A047211  Numbers that are congruent to {2, 4} mod 5.
 * https://oeis.org/A047211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + floor((n-1)/2)
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence’s offset */
    seq_t term = ((seq_t)n * 2) + (((seq_t)(n - 1)) / 2);
    return term;
}
