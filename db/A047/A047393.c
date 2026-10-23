/*
 * A047393  Numbers that are congruent to {0, 1} mod 8.
 * https://oeis.org/A047393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/2) + (n-1)%2
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A047393 – numbers congruent to {0, 1} mod 8, offset 1 */

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    idx_t m = n - 1;          /* shift so that the first term is 0 */
    seq_t term = (seq_t)8 * (seq_t)(m / 2) + (seq_t)(m % 2);
    return term;
}
