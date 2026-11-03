/*
 * A198398  a(n) = 8^n - 8n.
 * https://oeis.org/A198398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 8*n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 852 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    /* Compute 8^n using the provided helper */
    seq_t pow_val = ipow_u64_idx(8ULL, n);
    /* Compute 8*n as a seq_t to avoid overflow on the index */
    seq_t sub_val = (seq_t)8 * (seq_t)n;
    return pow_val - sub_val;
}
