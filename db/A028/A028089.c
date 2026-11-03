/*
 * A028089  Expansion of 1/((1-3x)(1-7x)(1-8x)(1-9x)).
 * https://oeis.org/A028089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*9^(n+3)-24*8^(n+3)+15*7^(n+3)-3^(n+3))/120
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    idx_t e = n + 3;
    seq_t t9 = ipow_u64_idx(9, e);
    seq_t t8 = ipow_u64_idx(8, e);
    seq_t t7 = ipow_u64_idx(7, e);
    seq_t t3 = ipow_u64_idx(3, e);

    seq_t res = t9 * 10;
    res -= t8 * 24;
    res += t7 * 15;
    res -= t3;
    res /= 120;
    return res;
}
