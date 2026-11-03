/*
 * A017153  a(n) = (8*n + 7)^5.
 * https://oeis.org/A017153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 7)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 9, u64 n <= 890, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 9888 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)8 * n) + 7;
    return ipow(base, (seq_t)5);
}
