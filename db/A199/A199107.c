/*
 * A199107  a(n) = 5*12^n - 1.
 * https://oeis.org/A199107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*12^n - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 876 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* compute 12^n */
    seq_t val = ipow((seq_t)12, (seq_t)n);

    /* multiply by 5 */
    val = val * 5;

    /* subtract 1 */
    val = val - 1;

    return val;
}
