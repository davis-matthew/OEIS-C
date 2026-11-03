/*
 * A017050  a(n) = (7*n + 5)^10.
 * https://oeis.org/A017050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 5)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 11, u128 n <= 1018,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 8949 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)7 * n + (seq_t)5;
    return ipow(base, (seq_t)10);
}
