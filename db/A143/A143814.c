/*
 * A143814  Triangle T(n,m) read along rows: T(n,m) = n^2 - (m+1)^2 for
 *          1<=m<n-1, T(n,n-1) = n^2-1.
 * https://oeis.org/A143814
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   T(n,m) = n^2 - (m+1)^2 for 1 ≤ m < n-1, T(n,n-1) = n^2 - 1. The
 *            sequence reads the triangle row‑by‑row starting at offset 2:
 *            a(2) = T(2,1) = 3
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* value below the offset is undefined */

    /* Find the row r such that r(r-1)/2 ≥ n-1, with r ≥ 2. */
    idx_t target = n - 1;              /* needed value in the inequality */
    idx_t low  = 2, high = 2;
    while (high * (high - 1) / 2 < target)
        high <<= 1;                    /* double until we pass the target */

    /* binary search for the smallest r */
    while (high - low > 1) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid - 1) / 2 >= target)
            high = mid;
        else
            low = mid;
    }
    idx_t r = high;

    /* Position within the row (0‑based). */
    idx_t prev = (r - 2) * (r - 1) / 2;        /* elements before row r */
    idx_t k0   = (n - 2) - prev;               /* 0‑based index in row r */
    idx_t m    = k0 + 1;                       /* column (1‑based) */

    /* Compute the value. */
    seq_t r_sq = (seq_t)r * (seq_t)r;
    if (m == r - 1)                          /* last element of the row */
        return r_sq - 1;
    else {
        seq_t t = (seq_t)(m + 1);
        return r_sq - t * t;
    }
}
