/*
 * A059409  a(n) = 4^n * (2^n - 1).
 * https://oeis.org/A059409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * (2^n - 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 98 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 0, so we can return early */
    if (n == 0) {
        return 0;
    }

    /* Compute 2^n and 2^(2n) = 4^n */
    seq_t two = 2;
    seq_t pow2n   = ipow(two, (seq_t)n);
    seq_t pow2_2n = ipow(two, (seq_t)(n << 1));

    /* a(n) = 4^n * (2^n - 1) */
    return pow2_2n * (pow2n - 1);
}
