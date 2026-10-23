/*
 * A136289  Start with three pennies touching each other on a tabletop. In
 *          each generation, add pennies subject to the rule that a penny can
 *          be placed only when (at least) two pennies are already in position
 *          to determine its position; sequence gives number of pennies added
 *          at generation n.
 * https://oeis.org/A136289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + 1 + ((n+2) mod 3)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nm = (seq_t)n;                /* convert index to seq_t */
    seq_t mod = (seq_t)((n + 2) % 3);   /* compute (n+2) mod 3, stays in idx_t */
    return nm * 2 + 1 + mod;
}
