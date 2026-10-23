/*
 * A187859  Number of 4-step one space for components leftwards or up, two
 *          space for components rightwards or down asymmetric quasi-queen's
 *          tours (antidiagonal moves become knight moves) on an n X n board
 *          summed over all starting positions.
 * https://oeis.org/A187859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 (n=1), 0 (n=2), 216 (n=3), 968 (n=4), 2754 (n=5), for
 *            n>5: a(n) = 497*n^2 - 2652*n + 3448
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 5)
    {
        /* a(1)=0, a(2)=0, a(3)=216, a(4)=968, a(5)=2754 */
        static const seq_t init[5] = {0, 0, 216, 968, 2754};
        return init[n - 1];
    }

    seq_t nn = (seq_t)n;
    return 497 * nn * nn - 2652 * nn + 3448;
}
