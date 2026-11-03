/*
 * A195116  a(n) = (2+3^n)*(3+2^n).
 * https://oeis.org/A195116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2+3^n)*(3+2^n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 902 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t pow2 = ipow((seq_t)2, (seq_t)n);
    seq_t pow3 = ipow((seq_t)3, (seq_t)n);
    return ((seq_t)2 + pow3) * ((seq_t)3 + pow2);
}
