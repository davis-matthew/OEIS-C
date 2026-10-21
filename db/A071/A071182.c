/*
 * A071182  a(n) = lpf(n+1) - lpf(n), where lpf = A020639.
 * https://oeis.org/A071182
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = lpf(n+1) - lpf(n) (offset 2)
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   lpf_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/lpf_uint64.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;                  /* below the offset – undefined     */
    uint64_t lp_n   = lpf_uint64(n);
    uint64_t lp_np1 = lpf_uint64(n + 1);
    int64_t  diff   = (int64_t)lp_np1 - (int64_t)lp_n;
    return (seq_t)diff;
}
