/*
 * A130249  Maximal index k of a Jacobsthal number such that A001045(k)<=n
 *          (the 'lower' Jacobsthal inverse).
 * https://oeis.org/A130249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_2(3*n+1))
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 8283 (2026-11-01)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bit_length.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 3 + 1;
    return (seq_t)(bit_length(m) - 1);
}
