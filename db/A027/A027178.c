/*
 * A027178  a(n) = T(n,0) + T(n,1) + ... + T(n,n), T given by A027170.
 * https://oeis.org/A027178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*2^n - 4*n - 8
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 32 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t pow2 = ipow_u64_idx(2, n);      /* 2^n */
    seq_t res = 9 * pow2;
    res -= 4 * ((seq_t)n);
    res -= 8;
    return res;
}
