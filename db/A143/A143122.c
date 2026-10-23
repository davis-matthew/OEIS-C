/*
 * A143122  Triangle read by rows, T(n,k) = Sum_{j=k..n} j!, 0 <= k <= n.
 * https://oeis.org/A143122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 629, big any n
 * Helpers:   floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143122: Triangle read by rows, T(n,k)=∑_{j=k..n} j!
   Here a(n) is the triangle read row‑wise. */

#include <stdint.h>
#include "../utils/floor_isqrt_u64.h"   /* provides floor_isqrt_u64 */

static idx_t find_row(idx_t n) {
    /* Find r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t lo = 0, hi = 1;
    while (( (__int128)hi * (hi + 1) / 2 ) <= n) {
        hi <<= 1;
    }
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (( (__int128)mid * (mid + 1) / 2 ) <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n) {
    /* Locate the row r and column k for index n. */
    idx_t r = find_row(n);                   /* row number */
    idx_t first = r * (r + 1) / 2;           /* first index of this row */
    idx_t k = n - first;                     /* column within the row */

    /* Compute Σ_{j=k..r} j! */
    seq_t sum = 0;
    seq_t fact = 1;                          /* 0! */
    if (k == 0)                             /* include 0! if needed */
        sum = fact;

    for (idx_t i = 1; i <= r; ++i) {
        fact *= (seq_t)i;                    /* i! */
        if (i >= k)
            sum += fact;                     /* add if within range */
    }
    return sum;
}
