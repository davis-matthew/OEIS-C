/*
 * A168617  a(n) = 7*2^(n-1) - 2*n - 5.
 * https://oeis.org/A168617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*2^(n-1) - 2*n - 5
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 910 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */
    seq_t res = 7 * pow2(n - 1);   /* 7 * 2^(n-1) */
    res -= 2 * (seq_t)n;           /* subtract 2*n */
    res -= 5;                      /* subtract 5 */
    return res;
}
