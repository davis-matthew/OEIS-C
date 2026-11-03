/*
 * A182990  Powers of c, the exact speed of light in vacuum (m/s).
 * https://oeis.org/A182990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 299792458^n
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 89 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    return ipow_u64_idx(299792458, n);
}
