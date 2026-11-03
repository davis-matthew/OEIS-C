/*
 * A017182  a(n) = (9n+1)^10.
 * https://oeis.org/A017182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9n+1)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 9 + 1;
    return ipow(base, 10);
}
