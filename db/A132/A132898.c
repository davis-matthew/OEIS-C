/*
 * A132898  Triangle read by rows: T(n,k) = (-1)^(n-1)*n + (-1)^(k-1)*k - 1, 1
 *          <= k <= n.
 * https://oeis.org/A132898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = (-1)^(r-1)*r + (-1)^(k-1)*k - 1 where r and k are the row
 *            and column of the (n‑th) entry read by rows of the triangle
 *            T(n,k) = (-1)^(n-1)*n + (-1)^(k-1)*k - 1
 * Range:     i8 n <= 1275, i16 n <= 1275, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1; a(2) = -2; a(3) = -5; a(4) = 3; a(5) = 0; ... */

#include <stdint.h>

static idx_t binary_search_row(idx_t n)
/* Return the smallest r such that r*(r+1)/2 >= n.  */
{
    idx_t low = 1, high = n, r = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n) {
            r = mid;
            if (mid == 0) break;   /* safety, mid cannot be 0 here */
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    /* 1‑based offset: a(1) is first term.  */
    /* Find row r.  */
    idx_t r = binary_search_row(n);

    /* Previous triangular total: (r-1)*r/2.  */
    __uint128_t prev_tri = (__uint128_t)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_tri;          /* 1‑based column */

    /* Signs: (-1)^(r-1) and (-1)^(k-1).  */
    int sgn_r = (r & 1) ? 1 : -1;
    int sgn_k = (k & 1) ? 1 : -1;

    /* Compute the entry.  */
    seq_t val = (seq_t)(sgn_r * (seq_t)r) + (seq_t)(sgn_k * (seq_t)k) - 1;
    return val;
}
