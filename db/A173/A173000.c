/*
 * A173000  a(n) = binomial(n + 4, 4)*9^n.
 * https://oeis.org/A173000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4,4)*9^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 400 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    seq_t comb = binomial(n + 4, 4);
    seq_t pow9 = ipow_i32(9, (int)n);
    return comb * pow9;
}
