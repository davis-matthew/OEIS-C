/*
 * A056090  Number of 4-element ordered antichain covers of an unlabeled
 *          n-element set.
 * https://oeis.org/A056090
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n+14,14) - 12*C(n+10,10) + 24*C(n+8,8) + 4*C(n+7,7) -
 *            18*C(n+6,6) + 6*C(n+5,5) - 36*C(n+4,4) + 36*C(n+3,3) +
 *            11*C(n+2,2) - 22*C(n+1,1) + 6*C(n,0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 22, u64 n <= 136, u128 n <= 1000,
 *            big any n
 * Helpers:   binom128_l
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom128_l.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;               /* offsets start at n = 4 */

    unsigned __int128 total = 0;

    total +=  binom128_l(n + 14, 14);
    total -= 12 * binom128_l(n + 10, 10);
    total += 24 * binom128_l(n + 8,  8);
    total +=  4 * binom128_l(n + 7,  7);
    total -= 18 * binom128_l(n + 6,  6);
    total +=  6 * binom128_l(n + 5,  5);
    total -= 36 * binom128_l(n + 4,  4);
    total += 36 * binom128_l(n + 3,  3);
    total += 11 * binom128_l(n + 2,  2);
    total -= 22 * binom128_l(n + 1,  1);
    total +=  6 * binom128_l(n,      0);   /* C(n,0) = 1 */

    return (seq_t)total;                    /* safe cast for n where result fits */
}
