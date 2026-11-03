/*
 * A013753  a(n) = 13^(3*n + 2).
 * https://oeis.org/A013753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13^(3*n + 2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 193 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    seq_t base = (seq_t)13;          /* the base 13 */
    idx_t  exp_idx = 3*n + 2;        /* exponent as integer index */
    seq_t exp = (seq_t)exp_idx;      /* cast to seq_t */
    return ipow(base, exp);       /* compute the power */
}
