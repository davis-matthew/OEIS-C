/*
 * A024079  a(n) = 7^n - n^4.
 * https://oeis.org/A024079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - n^4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 496 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* 7^n */
    seq_t pow7 = ipow_u64_idx(7ULL, n);

    /* n^4 */
    seq_t nn = (seq_t)n;
    seq_t n4 = nn * nn * nn * nn;

    return pow7 - n4;
}
