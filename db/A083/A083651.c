/*
 * A083651  Triangular array, read by rows: T(n,k) = k-th bit in binary
 *          representation of n (0<=k<=n).
 * https://oeis.org/A083651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   tri
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (r >> k) & 1, where
 *   r = floor((sqrt(8*n+1)-1)/2)  (the unique r with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2)
 *   k = n - r*(r+1)/2              (column within that row, 0 ≤ k ≤ r)
 *   r, k are computed with 64‑bit arithmetic; the bit is taken from the
 *   binary representation of r (least significant bit has index 0).
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/tri.h"

/* 64‑bit unsigned integer type used for indices */
typedef uint64_t idx_t;
/* seq_t is defined by the build system (e.g. uint8_t, uint64_t, etc.) */

seq_t a(idx_t n)
{
    /* Find the row r such that tri(r) ≤ n < tri(r+1) */
    idx_t low = 0;
    idx_t high = 1;

    /* Increase high until tri(high) > n */
    while (tri(high) <= n) {
        /* Prevent overflow of high when it gets very large */
        if (high > (UINT64_MAX >> 1)) {
            break;
        }
        high <<= 1;
    }

    /* Binary search on [low, high) to find r = low */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (tri(mid) <= n) {
            low = mid;
        } else {
            high = mid;
        }
    }

    idx_t r = low;
    idx_t k = n - tri(r);      /* column within the row (0 ≤ k ≤ r) */

    /* Retrieve the k‑th least significant bit of r.  The shift operates on
       a 64‑bit type (idx_t), which guarantees a defined result because
       k < width of idx_t.  The final result fits in seq_t as 0 or 1. */
    return (seq_t)((r >> k) & 1);
}
