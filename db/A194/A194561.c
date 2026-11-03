/*
 * A194561  Centered cube numbers: (n+1)^27 + n^27.
 * https://oeis.org/A194561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^27 + n^27
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 9503 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Compute (n + 1)^27.  n itself fits in seq_t, so cast directly. */
    seq_t base1 = (seq_t)(n + 1);
    seq_t pow1  = ipow(base1, (seq_t)27);

    /* Compute n^27.  For n = 0, n^27 is 0, which is handled correctly. */
    seq_t pow2  = ipow((seq_t)n, (seq_t)27);

    return pow1 + pow2;
}
