/*
 * A176031  a(n) = n^rad(n).
 * https://oeis.org/A176031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^rad(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_idx, radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/radical.h"

seq_t a(idx_t n) {
    /* rad(n) is the product of distinct prime factors of n */
    seq_t r = radical(n);
    /* exponent rad(n) fits in idx_t because rad(n) <= n */
    return ipow_idx((seq_t)n, (idx_t)r);
}
