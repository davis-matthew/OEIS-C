/*
 * A016748  a(n) = (2*n)^8.
 * https://oeis.org/A016748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)^8
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 7, u64 n <= 127, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 9551 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * (seq_t)2;
    return ipow(base, (seq_t)8);
}
