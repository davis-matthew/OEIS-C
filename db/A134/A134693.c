/*
 * A134693  a(n)=A133806(n)+A133806(n+6).
 * https://oeis.org/A134693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k) = 3*2^k , a(2k+1) = 9*2^k (offset 0)
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    idx_t k = n >> 1;            /* floor(n/2) */
    seq_t val = pow2(k);         /* 2^k as seq_t */
    val = val * 3;               /* 3*2^k   */
    if (n & 1UL)                 /* odd index → multiply by another 3 */
        val = val * 3;           /* 9*2^k   */
    return val;
}
