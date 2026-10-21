/*
 * A093051  Exponent of 2 in (3^n-3)*2^n.
 * https://oeis.org/A093051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence implementation for OEIS A093051:
 *   a(0) = 0
 *   for odd n ≥ 1 : a(n) = n
 *   for even n = 2k : a(2k) = a(k) + k + 1 + (k & 1)
 *
 * This follows directly from the sequence definition:
 * a(n) = v₂((3ⁿ - 3)·2ⁿ) and simplifies to the above recurrence.
 */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    if (n & 1ULL) {           /* odd index */
        return (seq_t)n;
    }
    idx_t half = n >> 1;      /* k = n/2 */
    seq_t val   = a(half);    /* recursive call */
    seq_t inc   = (seq_t)half + 1 + ((half & 1ULL) ? 1 : 0);
    return val + inc;
}
