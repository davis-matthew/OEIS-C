/*
 * A115342  a(n) = 1 + (n-6)*2^(n-1).
 * https://oeis.org/A115342
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n-6)*2^(n-1)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 33 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n < 6) return 0;               /* undefined below offset */
    seq_t p2   = ipow_u64_idx(2, n - 1);     /* 2^(n-1) */
    seq_t mult = (seq_t)(n - 6);         /* (n-6) as seq_t */
    return (p2 * mult) + 1;
}
