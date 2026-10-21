/*
 * A063122  Dimension of the space of weight 2n cusp forms for Gamma_0( 54 ).
 * https://oeis.org/A063122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4, a(n) = 18*n - 15 for n > 1
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) holds for n > 3 */

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1.  n == 0 is ignored. */
    if (n == 0) return 0;          /* safety for unexpected 0 index */
    if (n == 1) return (seq_t)4;   /* first term */

    /* For n > 1 the closed formula 18*n - 15 applies. */
    seq_t val = (seq_t)18 * (seq_t)n;   /* multiplication in seq_t */
    return val - (seq_t)15;
}
