/*
 * A177069  a(n) = 11^n + n^11.
 * https://oeis.org/A177069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n + n^11
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 877 (2026-11-01)
 * Helpers:   ipow, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Compute 11^n */
    seq_t term1 = ipow((seq_t)11, (seq_t)n);

    /* Compute n^11 */
    seq_t term2 = ipow_u64_idx(n, 11);

    /* Return the sum */
    return term1 + term2;
}
