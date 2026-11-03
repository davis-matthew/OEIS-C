/*
 * A008790  a(n) = n^(n+4).
 * https://oeis.org/A008790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(n+4)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 183 (2026-11-01)
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
    seq_t exp  = (seq_t)(n + 4);
    return ipow(base, exp);
}
