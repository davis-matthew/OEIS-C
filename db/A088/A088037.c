/*
 * A088037  Smallest square k == 1 (mod some n-th power), k > 1.
 * https://oeis.org/A088037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 (n=1), 9 (n=2), (2^n-2)^2/4 (n>2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n == 1)   return (seq_t)4;
    if (n == 2)   return (seq_t)9;

    /* compute 2^n */
    seq_t two_n = ipow_idx((seq_t)2, n);

    /* (2^n - 2) */
    seq_t diff = two_n - (seq_t)2;

    /* square */
    seq_t sq = diff * diff;

    /* divide by 4 (exact) */
    return sq / (seq_t)4;
}
