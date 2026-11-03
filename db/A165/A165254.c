/*
 * A165254  a(n) = 9 + n^17.
 * https://oeis.org/A165254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9 + n^17
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 13, u128 n <= 184,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 968 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    return (seq_t)9 + ipow((seq_t)n, (seq_t)17);
}
