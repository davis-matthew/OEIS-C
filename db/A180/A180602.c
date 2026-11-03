/*
 * A180602  a(n) = (2^(n+1) - 1)^n.
 * https://oeis.org/A180602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n+1) - 1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-01)
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
    /* Compute 2^(n+1) as a seq_t value */
    seq_t pow2 = ipow_u64_idx(2, n + 1);

    /* Subtract 1 to get the base (2^(n+1) - 1) */
    seq_t base = pow2 - (seq_t)1;

    /* Raise the base to the power n */
    seq_t exp = (seq_t)n;
    return ipow(base, exp);
}
