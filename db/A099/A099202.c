/*
 * A099202  a(n) = 2^floor(n^2/2).
 * https://oeis.org/A099202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^floor(n^2/2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 72 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute floor(n^2/2) using the index type to avoid overflow in seq_t */
    idx_t exp = (n * n) / 2;
    return pow2(exp);
}
