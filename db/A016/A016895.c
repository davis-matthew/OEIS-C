/*
 * A016895  a(n) = (5*n + 3)^11.
 * https://oeis.org/A016895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 3)^11
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    base = base * (seq_t)5 + (seq_t)3;
    return ipow(base, (seq_t)11);
}
