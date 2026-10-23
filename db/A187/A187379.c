/*
 * A187379  Number of 5-step S, NW and NE-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 69*n^2 - 322*n + 372 for n > 3; special cases for n =
 *            1,2,3
 * Range:     u8 n <= 4, u16 n <= 33, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)40;

    seq_t nn = (seq_t)n;
    seq_t res = ((seq_t)69) * nn * nn - ((seq_t)322) * nn + 372;
    return res;
}
