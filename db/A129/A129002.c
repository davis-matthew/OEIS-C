/*
 * A129002  a(n) = (n^3 + n^2)*2^n.
 * https://oeis.org/A129002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n^2)*2^n
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 19, u64 n <= 47, u128 n <= 107,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 934 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    /* Compute n^2 * (n+1) */
    seq_t val = (seq_t)n;
    val *= (seq_t)n;          /* n^2 */
    val *= (seq_t)(n + 1);    /* n^2 * (n+1) = n^3 + n^2 */
    /* Multiply by 2^n using the provided power utility */
    val *= ipow_u64_idx(2, n);
    return val;
}
