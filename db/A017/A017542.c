/*
 * A017542  a(n) = (12*n + 1)^10.
 * https://oeis.org/A017542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 1)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 6, u128 n <= 594,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 9895 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Compute the base (12*n + 1) as seq_t to avoid overflow. */
    seq_t base = (seq_t)12 * n + (seq_t)1;
    /* Raise the base to the 10th power using the provided big integer
       exponentiation routine. */
    return ipow(base, (seq_t)10U);
}
