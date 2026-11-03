/*
 * A008785  a(n) = (n+4)^n.
 * https://oeis.org/A008785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+4)^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 187 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n + (seq_t)4;
    return ipow(base, (seq_t)n);
}
