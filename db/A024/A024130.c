/*
 * A024130  a(n) = 11^n - n^3.
 * https://oeis.org/A024130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - n^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 292 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 11^n */
    seq_t pow11 = ipow((seq_t)11, (seq_t)n);

    /* n^3 (performed safely for all supported seq_t sizes) */
    seq_t nn = (seq_t)n;
    seq_t cube = nn * nn * nn;

    /* a(n) = 11^n - n^3 */
    return pow11 - cube;
}
