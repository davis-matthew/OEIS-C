/*
 * A052944  a(n) = 2^n + n - 1.
 * https://oeis.org/A052944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + n - 1 (offset 0)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t pow2 = (seq_t)1;                 /* 2^0 */
    for (idx_t i = 0; i < n; ++i) {       /* compute 2^n by doubling */
        pow2 = pow2 + pow2;
    }
    return pow2 + (seq_t)n - 1;            /* a(n) = 2^n + n - 1 */
}
