/*
 * A071182  a(n) = lpf(n+1) - lpf(n), where lpf = A020639.
 * https://oeis.org/A071182
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = lpf(n+1) - lpf(n) (offset 2)
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 86 (2026-11-01)
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;                  /* below the offset – undefined     */
    uint64_t lp_n   = smallest_prime_factor(n);
    uint64_t lp_np1 = smallest_prime_factor(n + 1);
    int64_t  diff   = (int64_t)lp_np1 - (int64_t)lp_n;
    return (seq_t)diff;
}
