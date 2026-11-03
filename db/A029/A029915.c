/*
 * A029915  Convert n from yards to meters.
 * https://oeis.org/A029915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1143*n + 625)/1250)
 * Range:     u8 n <= 278, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9888 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_seq = n;
    seq_t val = (n_seq * 1143 + 625) / 1250;
    return val;
}
