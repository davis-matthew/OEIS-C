/*
 * A135540  a(n) = 2^(A000523(n) - A000120(n) + 2) - 1.
 * https://oeis.org/A135540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(floor(log_2(n)) - (n - Sum_{k=1,..,n}[floor(n/2^k)]) +
 *            2) - 1
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"


seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* below offset, value irrelevant */

    /* compute floor(log2(n)) */
    idx_t l = 0;
    idx_t tmp = n;
    while (tmp >> 1) {
        l++;
        tmp >>= 1;
    }

    /* compute popcount(n) */
    idx_t pc = 0;
    tmp = n;
    while (tmp) {
        pc += tmp & 1;
        tmp >>= 1;
    }

    /* exponent = floor(log2(n)) - popcount(n) + 2 */
    int64_t shift = (int64_t)l - (int64_t)pc + 2;
    if (shift <= 0) shift = 1;   /* safety guard, should never happen */

    seq_t result = pow2((idx_t)shift);
    result -= (seq_t)1;
    return result;
}
