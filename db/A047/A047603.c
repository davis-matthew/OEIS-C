/*
 * A047603  Numbers that are congruent to {1, 2, 3, 4, 5} mod 8.
 * https://oeis.org/A047603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 3*floor((n-1)/5)
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, but return 0 for safety */
    /* Compute a(n) = n + 3 * ((n-1) / 5) using integer arithmetic. */
    seq_t t = ((seq_t)(n - 1)) / 5;          /* floor((n-1)/5) */
    return (seq_t)n + (seq_t)3 * t;
}
