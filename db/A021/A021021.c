/*
 * A021021  Expansion of 1/((1-10x)(1-11x)(1-12x)).
 * https://oeis.org/A021021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*12^(n+1) - 11^(n+2) + 5*10^(n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t p12 = ipow_idx((seq_t)12, n + 1);
    seq_t p11 = ipow_idx((seq_t)11, n + 2);
    seq_t p10 = ipow_idx((seq_t)10, n + 1);

    seq_t res = (seq_t)6 * p12;
    res -= p11;
    res += (seq_t)5 * p10;
    return res;
}
