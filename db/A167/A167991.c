/*
 * A167991  Blocks of size 2n, each with 2n-1 replicas of 2n followed by 2n+1;
 *          n=1, 2, 3, ...
 * https://oeis.org/A167991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 2m if n < m(m+1) * a(n) = 2m + 1 if n == m(m+1) * where
 *            m is the smallest integer with m(m+1) >= n. * This generates
 *            blocks of length 2m: the first 2m–1 terms are 2m * and the last
 *            term of the block is 2m+1
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* treat n==0 as 0 (offset starts at 1, but this is harmless) */
    if (n == 0)
        return 0;

    /* binary search for the minimal block number m such that m*(m+1) >= n */
    idx_t low = 1;
    idx_t high = n;   /* n <= n*(n+1) always holds for n >= 1 */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t m = low;
    unsigned __int128 prod = (unsigned __int128)m * (m + 1);

    /* compute the sequence value */
    seq_t val = (seq_t)m * 2;
    if (prod == (unsigned __int128)n)
        val += 1;
    return val;
}
