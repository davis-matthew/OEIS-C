/*
 * A115450  Number triangle (1/((1-x)(1-2x)),-x)-(x/((1-x)(1-2x)),-x^2)
 *          (expressed in the notation of Riordan arrays).
 * https://oeis.org/A115450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 27, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 76 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115450 – Number triangle (1/((1-x)(1-2x)),-x)-(x/((1-x)(1-2x)),-x^2) */
/* a(n) = T(r,k);  r = row_of(n), k = n - r(r+1)/2                     */
/* T(n,k) = (2^(n-k+1)-1 - (2^(n-2k)-1) * (k <= n/2)) * (-1)^k        */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"

static inline idx_t
row_of(idx_t n)                 /* return r with r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
{
    idx_t low  = 0;
    idx_t high = 1;
    /* exponential search to find an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        low  = high;
        high <<= 1;              /* double high */
    }
    /* binary search between low (inclusive) and high (inclusive) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1) + 1; /* upper mid to avoid infinite loop */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine the row r and position k within that row. */
    idx_t r = row_of(n);
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Compute the first part: 2^(r-k+1) - 1. */
    seq_t part1 = ipow_idx((seq_t)2, (idx_t)(r - k + 1)) - (seq_t)1;

    /* Compute the second part if k <= floor(r/2). */
    seq_t part2 = 0;
    if (k <= r / 2)
    {
        part2 = ipow_idx((seq_t)2, (idx_t)(r - 2 * k)) - (seq_t)1;
    }

    /* Combine parts and apply the sign (-1)^k. */
    seq_t val = part1 - part2;
    if (k & 1)
        val = -val;

    return val;
}
