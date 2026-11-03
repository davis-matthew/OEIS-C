/*
 * A118108  Decimal representation of n-th iteration of the Rule 54 elementary
 *          cellular automaton starting with a single black cell.
 * https://oeis.org/A118108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((16+12*(n mod 2)) * 4^n / 15)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 1641 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* constants for the two cases */
    const seq_t FACTOR_EVEN = (seq_t)16;          /* 16 + 12*0 */
    const seq_t FACTOR_ODD  = (seq_t)28;          /* 16 + 12*1 */
    const seq_t DENOM      = (seq_t)15;

    /* choose factor based on parity of n */
    seq_t factor = ((n & 1) ? FACTOR_ODD : FACTOR_EVEN);

    /* compute 4^n (big_pow handles arbitrary precision) */
    seq_t pow4 = ipow((seq_t)4, (seq_t)n);

    /* perform division by 15 safely: q = floor(pow4/15), r = pow4 % 15 */
    seq_t q = pow4 / DENOM;
    seq_t r = pow4 % DENOM;

    /* compute (factor * pow4) / 15 without overflow */
    seq_t res = q * factor + (r * factor) / DENOM;
    return res;
}
