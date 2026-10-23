/*
 * A194488  Number of ways to arrange 4 nonattacking knights on the lower
 *          triangle of an n X n board.
 * https://oeis.org/A194488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^8 + 4*n^7 - 102*n^6 + 160*n^5 + 4713*n^4 * - 26612*n^3
 *            - 34564*n^2 + 669648*n - 1597824) / 384 * for n > 7; initial
 *            values for n ≤ 7: * a(1)=0, a(2)=0, a(3)=4, a(4)=46, a(5)=407, *
 *            a(6)=2168, a(7)=8685
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 33, u64 n <= 117, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 u128;

seq_t a(idx_t n)
{
    /* initial terms */
    static const seq_t small[] = {0, 0, 4, 46, 407, 2168, 8685};
    if (n <= 7) {
        return small[n - 1];
    }

    /* compute using the empirical polynomial (exact for n ≥ 8) */
    u128 x = (u128)n;
    u128 x2 = x * x;
    u128 x3 = x2 * x;
    u128 x4 = x3 * x;
    u128 x5 = x4 * x;
    u128 x6 = x5 * x;
    u128 x7 = x6 * x;
    u128 x8 = x7 * x;

    u128 num = x8
             + (u128)4   * x7
             - (u128)102 * x6
             + (u128)160 * x5
             + (u128)4713 * x4
             - (u128)26612 * x3
             - (u128)34564 * x2
             + (u128)669648 * x
             - (u128)1597824;

    return (seq_t)(num / (u128)384);
}
