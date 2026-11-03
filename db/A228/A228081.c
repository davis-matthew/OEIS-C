/*
 * A228081  a(n) = 64^n + 1.
 * https://oeis.org/A228081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 64^n + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 99 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t result = ipow((seq_t)64, (seq_t)n);
    result += (seq_t)1;
    return result;
}
