/*
 * A187512  Number of 7-step S, E, and NW-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 285*n^2 - 1624*n + 2210 for n ≥ 6; special values:
 *            a(1)=0, a(2)=0, a(3)=20, a(4)=330, a(5)=1224
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1 || n == 2) return (seq_t)0;
    if (n == 3)          return (seq_t)20;
    if (n == 4)          return (seq_t)330;
    if (n == 5)          return (seq_t)1224;

    /* n ≥ 6 */
    seq_t nn = (seq_t)n;
    seq_t term1 = (seq_t)285 * nn * nn;
    seq_t term2 = (seq_t)1624 * nn;
    return term1 - term2 + (seq_t)2210;
}
