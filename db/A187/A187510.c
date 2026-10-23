/*
 * A187510  Number of 5-step S, E, and NW-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 51*n^2 - 208*n + 200 for n>3, a(1) = 0, a(2) = 0, a(3) =
 *            40
 * Range:     u8 n <= 4, u16 n <= 37, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    if (n == 2)
        return 0;
    if (n == 3)
        return 40;

    seq_t nn = (seq_t)n;
    return (seq_t)51 * nn * nn - (seq_t)208 * nn + (seq_t)200;
}
