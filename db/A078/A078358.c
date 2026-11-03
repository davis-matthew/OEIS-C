/*
 * A078358  Non-oblong numbers: Complement of A002378.
 * https://oeis.org/A078358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(sqrt(n)) + n - 1
 * Range:     u8 n <= 240, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 7712 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    idx_t f = isqrt(n);
    idx_t c = ((unsigned __int128)f * f == n) ? f : f + 1;  /* ceil sqrt */
    return (seq_t)c + (seq_t)(n - 1);
}
