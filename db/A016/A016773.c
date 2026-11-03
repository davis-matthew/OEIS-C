/*
 * A016773  a(n) = (3*n)^9.
 * https://oeis.org/A016773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)^9
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 46, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 897 (2026-11-01)
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
    base *= 3;
    return ipow(base, 9u);
}
