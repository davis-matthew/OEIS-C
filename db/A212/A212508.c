/*
 * A212508  Number of (w,x,y,z) with all terms in {1,...,n} and w<2x and y<3z.
 * https://oeis.org/A212508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (#pairs (w,x) with w<2x) * (#pairs (y,z) with y<3z) where
 *            for k=2 or 3, the number of pairs is k*⌊(n+1)/k⌋*(⌊(n+1)/k⌋+1)/2
 *            - ⌊(n+1)/k⌋ + (n-⌊(n+1)/k⌋)*n Initial values: a(0)=0, a(1)=1,
 *            a(2)=12,
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pair_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pair_count.h"

seq_t a(idx_t n)
{
    seq_t cnt2 = pair_count(n, 2);
    seq_t cnt3 = pair_count(n, 3);
    return cnt2 * cnt3;
}
