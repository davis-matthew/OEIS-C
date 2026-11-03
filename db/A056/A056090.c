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
 * Validated: standard, pass, 42 terms through n = 972 (2026-11-01)
 * Helpers:   binomial_idx_to_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial_idx_to_u128.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;               /* offsets start at n = 4 */

    unsigned __int128 total = 0;

    total +=  binomial_idx_to_u128(n + 14, 14);
    total -= 12 * binomial_idx_to_u128(n + 10, 10);
    total += 24 * binomial_idx_to_u128(n + 8,  8);
    total +=  4 * binomial_idx_to_u128(n + 7,  7);
    total -= 18 * binomial_idx_to_u128(n + 6,  6);
    total +=  6 * binomial_idx_to_u128(n + 5,  5);
    total -= 36 * binomial_idx_to_u128(n + 4,  4);
    total += 36 * binomial_idx_to_u128(n + 3,  3);
    total += 11 * binomial_idx_to_u128(n + 2,  2);
    total -= 22 * binomial_idx_to_u128(n + 1,  1);
    total +=  6 * binomial_idx_to_u128(n,      0);   /* C(n,0) = 1 */

    return (seq_t)total;                    /* safe cast for n where result fits */
}
