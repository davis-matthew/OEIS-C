/*
 * A181839  Minimum of { k<n | k>0 is strong prime to n}, or zero if this set
 *          is empty.
 * https://oeis.org/A181839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the smallest k with 0 < k < n such that gcd(k,n)=1 and k does not divide n‑1.
   If no such k exists, return 0.  For n==1 the convention from OEIS is a(1)=1. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;          /* OEIS special case */

    idx_t half = n - 1;            /* n-1, safe since n>0 */
    for (idx_t k = 1; k < n; ++k)
    {
        /* strong prime condition */
        if (gcd((seq_t)k, (seq_t)n) == 1 && half % k != 0)
            return (seq_t)k;
    }
    return 0;
}
