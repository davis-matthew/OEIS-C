/*
 * A130072  a(n) = 5^n - 3^n - 2^n.
 * https://oeis.org/A130072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - 3^n - 2^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* For n ≥ 1; a(1) = 0, a(2) = 12, … */
    seq_t pow5 = ipow_u64_idx(5, n);
    seq_t pow3 = ipow_u64_idx(3, n);
    seq_t pow2 = ipow_u64_idx(2, n);

    seq_t result = pow5 - pow3;
    result -= pow2;
    return result;
}
