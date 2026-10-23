/*
 * A183982  1/4 the number of (n+1) X 6 binary arrays with all 2 X 2 subblock
 *          sums the same.
 * https://oeis.org/A183982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 3*2^(n/2-1) + 2^(n-1) + 22 for even n * a(n) = 2^(n-1)
 *            + 2^((n+1)/2) + 22 for odd n * (an integer expression valid for
 *            all n ≥ 1) * * The function uses the utility `pow2` to compute
 *            powers of two. * No memoisation or recursion is required; each
 *            call runs in constant time
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the (unlikely) case n == 0 by returning 0. */
    if (n == 0) return 0;

    if (n % 2 == 0)  /* even n */
    {
        seq_t term = 22
                   + pow2(n - 1)
                   + (seq_t)3 * pow2(n / 2 - 1);
        return term;
    }
    else            /* odd n */
    {
        seq_t term = 22
                   + pow2(n - 1)
                   + pow2((n + 1) / 2);
        return term;
    }
}
