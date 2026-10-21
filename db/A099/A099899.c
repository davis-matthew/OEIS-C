/*
 * A099899  Multiplies by 4 and shifts right under the XOR BINOMIAL transform
 *          (A099898).
 * https://oeis.org/A099899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = SumXOR_{k=0..n} (C(n-k + [k/2], [k/2]) mod 2) * 4^k
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t b   = k >> 1;               /* floor(k/2)          */
        idx_t a_t = n - k + b;           /* n-k + floor(k/2)    */
        /* binomial coefficient modulo 2 is odd iff (b & (a_t - b)) == 0 */
        if ((b & (a_t - b)) == 0) {
            result ^= pow4(k);           /* 4^k  (arbitrary-precision) */
        }
    }
    return result;
}
