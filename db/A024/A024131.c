/*
 * A024131  a(n) = 11^n - n^4.
 * https://oeis.org/A024131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - n^4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 297 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* compute 11^n using the arbitrary‑precision helper */
    seq_t pow = ipow((seq_t)11, (seq_t)n);

    /* compute n^4 in seq_t */
    seq_t nn = (seq_t)n;
    seq_t n4 = nn * nn * nn * nn;

    return pow - n4;
}
