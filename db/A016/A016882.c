/*
 * A016882  a(n) = (5n+2)^10.
 * https://oeis.org/A016882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 2)^10
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 16, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 995 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)5 * n + 2;
    return ipow(base, 10);
}
