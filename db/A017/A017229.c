/*
 * A017229  a(n) = (9*n + 5)^9.
 * https://oeis.org/A017229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 5)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 14, u128 n <= 2123,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 9139 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)((seq_t)9 * n + 5);
    return ipow(base, (seq_t)9);
}
