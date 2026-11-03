/*
 * A145516  11^1,22^2,33^3,44^4,...
 * https://oeis.org/A145516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (11*n)^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)11 * (seq_t)n;
    return ipow(base, (seq_t)n);
}
