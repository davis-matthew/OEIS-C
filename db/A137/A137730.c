/*
 * A137730  Number of circular permutations of the multiset {1,1,2,2,...,n,n}
 *          (up to rotations) with odd distances between equal elements.
 * https://oeis.org/A137730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   mul_or_cancel
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n!)^2 / (2n)     for even n
 *            a(n) = (n!)^2 + n!) / (2n)  for odd n
 * The implementation computes the even part by cancelling the
 * denominator 2n during two passes over the factors 1 … n.
 * For odd n an additional term ((n-1)!)/2 is added.
 */
#include <stdint.h>
#include "../utils/mul_or_cancel.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* offset starts at 1 – guard for safety */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t denom = (seq_t)n;
    denom *= (seq_t)2;        /* 2n */
    seq_t res = (seq_t)1;     /* will hold the even part */

    for (idx_t i = 1; i <= n; ++i) {
        mul_or_cancel(&res, &denom, i);  /* first copy of i   */
        mul_or_cancel(&res, &denom, i);  /* second copy of i  */
    }

    if ((n & 1ULL) == 0)          /* even n */
        return res;

    /* odd n: add ((n-1)!)/2 */
    seq_t add = (seq_t)1;
    for (idx_t k = 1; k < n; ++k)
        add *= (seq_t)k;
    add /= (seq_t)2;
    return res + add;
}
