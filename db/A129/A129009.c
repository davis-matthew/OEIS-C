/*
 * A129009  a(n) = (n^3 + n^2)*9^n.
 * https://oeis.org/A129009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n^2) * 9^n, n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 998 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; undefined for n < 1 */
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t pow9 = ipow_idx((seq_t)9, n);     /* 9^n */

    /* term = n^2 * (n + 1) */
    seq_t term = nn * nn;         /* n^2 */
    term = term * (nn + 1);       /* n^2 * (n+1) */

    return term * pow9;           /* (n^3 + n^2) * 9^n */
}
