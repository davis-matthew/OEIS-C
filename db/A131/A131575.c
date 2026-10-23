/*
 * A131575  First differences of A131572.
 * https://oeis.org/A131575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0 0 if n is even and n > 0 2^((n-1)/2) if n is
 *            odd
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided power‐of‐two helper */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return pow2(0);            /* a(0) = 1 */
    if ((n & 1) == 0)
        return 0;                 /* even n > 0 */
    return pow2((n - 1) / 2);      /* odd n */
}
