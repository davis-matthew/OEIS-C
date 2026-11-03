/*
 * A187879  a(n) = 3^(n^3).
 * https://oeis.org/A187879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n^3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: standard, pass, 13 terms through n = 12 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* exponent = n^3 */
    seq_t nn = (seq_t)n;
    seq_t exp = nn * nn * nn;
    return ipow((seq_t)3, exp);
}
