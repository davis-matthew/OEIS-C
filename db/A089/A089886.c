/*
 * A089886  T(n,k) = number of subsets of {1,..., n} containing exactly k
 *          squares, triangle read by rows, 0<=k<n.
 * https://oeis.org/A089886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = T(row,k)  where  row and k are obtained from the one‑based
    index n by reading the triangle T row by row (k runs from 0 to row‑1).
    T(row,k) = binomial( floor(√row), k ) * 2^(row – floor(√row))
    for 0 ≤ k ≤ floor(√row); otherwise T(row,k) = 0.          */

#include <stddef.h>
#include "../utils/binomial.h"

static idx_t floor_isqrt(idx_t n) {
    /* 1‑based binary search for the floor of the square root of n. */
    if (n < 2) return n;
    idx_t lo = 1, hi = (n >> 1) + 1, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid <= n / mid) {      /* mid^2 ≤ n  */
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

static idx_t find_row(idx_t n) {
    idx_t lo = 1, hi = n;              /* upper bound: row ≤ n  */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= (unsigned __int128)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;                          /* row number (1‑based) */
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;                /* behaviour below offset is irrelevant */

    /* Find row and column corresponding to the flattened index n. */
    idx_t row = find_row(n);                                 /* 1‑based */
    unsigned __int128 prev = (unsigned __int128)(row - 1) * row / 2;
    idx_t k = (idx_t)(n - prev - 1);                         /* 0‑based */

    /* Compute T(row,k). */
    idx_t s = floor_isqrt(row);                               /* floor(√row) */

    if (k > s)                                                /* outside range */
        return (seq_t)0;

    seq_t bin = binomial(s, k);                               /* C(s,k) */

    /* power of two: 2^(row - s) */
    idx_t exp = row - s;
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        pow2 *= (seq_t)2;

    return bin * pow2;
}
