/*
 * A180964  a(0)=1; for n>0, a(n) = 1 + 3*A117571(n-1).
 * https://oeis.org/A180964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 2 if ((n+1)%3 == 0); otherwise a(n) = 3*n + 1. This
 *            follows the OEIS formula a(n) = 3*n - 2*cos(2*Pi*(n+1)/3) but
 *            uses integer arithmetic
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n+1) mod 3 */
    seq_t r = ((seq_t)n + 1) % 3;

    /* Compute 3*n */
    seq_t three_n = (seq_t)n * 3;

    /* Return the appropriate value */
    return (r == 0) ? (three_n - 2) : (three_n + 1);
}
