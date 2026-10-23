/*
 * A036799  a(n) = 2 + 2^(n+1)*(n-1).
 * https://oeis.org/A036799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 2^(n+1) * (n-1) (OEIS A036799)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t power = pow2(n + 1);            /* compute 2^(n+1) */
    return ((seq_t)(n - 1)) * power + 2;   /* (n-1)*2^(n+1) + 2 */
}
