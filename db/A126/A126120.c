/*
 * A126120  Catalan numbers (A000108) interpolated with 0's.
 * https://oeis.org/A126120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n is even) ? Catalan(n/2) : 0
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 73, u128 n <= 139,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n & 1)     /* odd index → 0 */
        return 0;
    idx_t k = n >> 1;   /* n/2 */
    return catalan(k);
}
