/*
 * A101553  A modular recurrence.
 * https://oeis.org/A101553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=5; for n>=0, a(2n) = 15^n, a(2n+1) = 5·15^n. This
 *            is a direct implementation of the closed form, so the function
 *            runs in O(log n) using binary exponentiation
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   power_of_15
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power_of_15.h"

/* Public sequence function. */
seq_t a(idx_t n)
{
    idx_t half = n >> 1;            /* n/2 */
    if (n & 1U)                     /* odd n : a(2k+1) = 5*15^k */
        return (seq_t)5 * power_of_15(half);
    else                            /* even n : a(2k)   = 15^k */
        return power_of_15(half);
}
