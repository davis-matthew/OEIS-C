/*
 * A013852  a(n) = 9^(5*n + 3).
 * https://oeis.org/A013852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^(5*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 80 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    idx_t exponent = n * 5 + 3;
    return ipow((seq_t)9, (seq_t)exponent);
}
