/*
 * A187860  Number of 5-step one space for components leftwards or up, two
 *          space for components rightwards or down asymmetric quasi-queen's
 *          tours (antidiagonal moves become knight moves) on an n X n board
 *          summed over all starting positions.
 * https://oeis.org/A187860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3763*n^2 - 25044*n + 40644 for n > 7 (the sequence
 *            satisfies a(n)=3*a(n-1)-3*a(n-2)+a(n-3) for n>10). Small n:
 *            a(1)=0, a(2)=0, a(3)=486, a(4)=3320, a(5)=11986, a(6)=26836,
 *            a(7)=50378
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return known values for n <= 7 */
    static const seq_t small[8] = {0, 0, 0, 486, 3320, 11986, 26836, 50378};

    if (n <= 7) {
        return small[(idx_t)n];
    }

    /* Formula for n > 7 */
    seq_t nn = (seq_t)n;
    return (seq_t)3763 * nn * nn
          - (seq_t)25044 * nn
          + (seq_t)40644;
}
