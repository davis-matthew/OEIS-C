/*
 * A047623  Numbers that are congruent to {1, 3, 5} mod 8.
 * https://oeis.org/A047623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor((n-1)/3) + 2*n - 1
 * Range:     u8 n <= 96, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (n-1)/3 is computed in idx_t; then cast to seq_t for the final arithmetic */
    idx_t k = (n - 1) / 3;              /* floor((n-1)/3) */
    seq_t result = (seq_t)k * 2 + (seq_t)n * 2 - 1;
    return result;
}
