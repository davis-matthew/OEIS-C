/*
 * A228879  a(n+2) = 3*a(n), starting 4,7.
 * https://oeis.org/A228879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*3^(n/2) if n is even, a(n) = 7*3^((n-1)/2) if n is odd
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 37, u64 n <= 78, u128 n <= 158,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 952 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0ULL) {
        idx_t k = n >> 1;
        return ipow_idx((seq_t)3, k) * (seq_t)4;
    } else {
        idx_t k = (n - 1ULL) >> 1;
        return ipow_idx((seq_t)3, k) * (seq_t)7;
    }
}
