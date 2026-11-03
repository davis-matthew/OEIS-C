/*
 * A016786  a(n) = (3*n+1)^10.
 * https://oeis.org/A016786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 27, u128 n <= 2376,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 8993 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 3 + 1;
    return ipow(base, (seq_t)10);
}
