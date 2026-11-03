/*
 * A164090  a(n) = 2*a(n-2) for n > 2; a(1) = 2, a(2) = 3.
 * https://oeis.org/A164090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) for n > 2; a(1) = 2, a(2) = 3
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Validated: standard, pass, 58 terms through n = 1780 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    if (n & 1) {
        /* odd indices: a(n) = 2^((n+1)/2) */
        idx_t exp = (n + 1) >> 1;
        return ipow_u64_idx(2, exp);
    } else {
        /* even indices: a(n) = 3 * 2^((n/2) - 1) */
        idx_t exp = (n >> 1) - 1;
        seq_t p = ipow_u64_idx(2, exp);
        return (seq_t)3 * p;
    }
}
