/*
 * A107392  Number of (inequivalent) fuzzy subgroups of the direct sum of
 *          group of integers modulo p^n and group of integers modulo 2 for a
 *          prime p with (p,2) = 1. Z_{p^n} + Z_2.
 * https://oeis.org/A107392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (n^2 + 7n + 8) - 1
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 22, u64 n <= 52, u128 n <= 114,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t pow  = pow2(n);                 // 2^n
    seq_t base = sn * sn + (seq_t)7 * sn + (seq_t)8;
    return pow * base - (seq_t)1;
}
