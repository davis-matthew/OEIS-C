/*
 * A013760  a(n) = 17^(3*n + 1).
 * https://oeis.org/A013760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17^(3*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 4, u128 n <= 10,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 79 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = 17;
    seq_t exp = (seq_t)(3*n + 1);
    return ipow(base, exp);
}
