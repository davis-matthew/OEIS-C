/*
 * A239632  Number of parts in all palindromic compositions of n.
 * https://oeis.org/A239632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1 For n >= 2: If n is even, let k = n/2 (k >
 *            0): a(n) = (2k + 1) * 2^(k-1) If n is odd, let k = (n-1)/2 (k >=
 *            1): a(n) = (2k + 2) * 2^(k-1)
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 55, u64 n <= 117,
 *            u128 n <= 243, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2)               /* a(0) = 0, a(1) = 1 */
        return (seq_t)n;

    if ((n & 1u) == 0) {     /* even n = 2k, k > 0 */
        idx_t k = n >> 1;    /* k = n/2 */
        return (seq_t)(2 * k + 1) * pow2(k - 1);
    } else {                 /* odd n = 2k+1, k >= 1 */
        idx_t k = (n - 1) >> 1;  /* k = (n-1)/2 */
        return (seq_t)(2 * k + 2) * pow2(k - 1);
    }
}
