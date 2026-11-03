/*
 * A199321  a(n) = 7*6^n+1.
 * https://oeis.org/A199321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*6^n+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 864 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t pow6 = ipow((seq_t)6, (seq_t)n); /* compute 6^n */
    seq_t res  = pow6 * (seq_t)7;             /* multiply by 7 */
    res += (seq_t)1;                          /* add 1 */
    return res;
}
