/*
 * A016276  Expansion of 1/((1-2x)(1-3x)(1-7x)).
 * https://oeis.org/A016276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (16*2^n - 45*3^n + 49*7^n)/20
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   ipow_idx, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t pow2_n = pow2(n);
    seq_t pow3_n = pow3(n);
    seq_t pow7_n = ipow_idx((seq_t)7, n);

    seq_t num = pow2_n * (seq_t)16;
    num += pow7_n * (seq_t)49;
    num -= pow3_n * (seq_t)45;

    return num / (seq_t)20;
}
