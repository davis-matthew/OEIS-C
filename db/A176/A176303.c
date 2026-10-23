/*
 * A176303  a(n) = abs(2^n-127).
 * https://oeis.org/A176303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = abs(2^n - 127)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p = pow2(n);          /* compute 2^n */
    if (n <= 6)                 /* 2^n <= 64, so we subtract from 127 */
        return 127 - p;
    else                        /* for n > 6, 2^n > 127 */
        return p - 127;
}
