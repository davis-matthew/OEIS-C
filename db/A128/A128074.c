/*
 * A128074  a(n) = (n^3+n)*9^n.
 * https://oeis.org/A128074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n) * 9^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 847 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* For n = 0 the result is 0. */
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;          /* n^2   */
    seq_t factor = sq + 1;       /* n^2+1 */
    seq_t mult = sn * factor;    /* n*(n^2+1) = n^3+n */
    seq_t pow9 = ipow((seq_t)9, n); /* 9^n   */
    return mult * pow9;
}
