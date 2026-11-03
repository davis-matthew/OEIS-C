/*
 * A128989  a(n) = (n^3 - n^2)*6^n.
 * https://oeis.org/A128989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n^2) * 6^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 19, u128 n <= 43,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 996 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t N = n;                     /* cast index to seq_t */
    seq_t pow6 = ipow_u64_idx(6, n);      /* 6^n */
    seq_t val = N * N;                /* n^2 */
    val *= (N - 1);                   /* * (n-1) */
    val *= pow6;                      /* * 6^n */
    return val;
}
