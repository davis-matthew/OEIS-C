/*
 * A127368  Relative prime triangle, read by rows.
 * https://oeis.org/A127368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) returns the k-th entry of the lower triangular array where
 *            T(r,k) = k if gcd(r,k)=1 else 0, read by rows. Index n is
 *            1‑based overall in the flattened triangle
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static idx_t
row_of(idx_t n) {
    idx_t lo = 1, hi = 1;
    /* find an upper bound hi such that hi*(hi+1)/2 >= n */
    while (((unsigned __int128)hi * (hi + 1)) / 2 < n) {
        hi <<= 1;
    }
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 >= n) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

seq_t
a(idx_t n) {
    /* Determine the row r and the column k within that row. */
    idx_t r = row_of(n);
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - prev;              /* 1‑based column index */
    seq_t g = gcd((seq_t)r, (seq_t)k);
    if (g == (seq_t)1) {
        return (seq_t)k;
    } else {
        return (seq_t)0;
    }
}
