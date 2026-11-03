/*
 * A124399  a(n) = 4^(n - bitcount(n)) where bitcount(n) = A000120(n).
 * https://oeis.org/A124399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n - A000120(n)) (A124399)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   ipow_u64_idx, popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/popcount.h"

seq_t a(idx_t n) {
    int pc = popcount(n);           /* bitcount(n), A000120 */
    idx_t exp = n - (idx_t)pc;          /* exponent = n - bitcount(n) */
    return ipow_u64_idx(4, exp);            /* compute 4^exp in seq_t type */
}
