/*
 * A017001  a(n) = (7*n + 1)^9.
 * https://oeis.org/A017001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 1)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 19, u128 n <= 2730,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 9733 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)7 * n + 1;
    return ipow(base, (seq_t)9);
}
