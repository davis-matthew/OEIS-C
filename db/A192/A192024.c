/*
 * A192024  Triangle read by rows: T(n,k) is the number of unordered pairs of
 *          nodes at distance k in the double-comb graph \/_\/_\/...\/_\/ with
 *          3n (n>=1) nodes. The entries in row n are the coefficients of the
 *          corresponding Wiener polynomial.
 * https://oeis.org/A192024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th entry of the triangle T(n,k) read by rows.
 *            T(row,1)=3*row-1; T(row,2)=6*row-6 (row>1), T(1,2)=1; For k>=3:
 *            let m=row-(k-1); if m==0 ->4, m==1 ->12, else ->9*m+3
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for uint64_t using binary search. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;               /* sqrt(2^64‑1) < 2^32 */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Return the row index (1‑based) containing the n‑th triangle entry. */
static inline idx_t triangle_row(idx_t n)
{
    idx_t sqrtD = isqrt_u64(9 + 8 * n);  /* floor sqrt of 9+8n */
    idx_t r = sqrtD >= 3 ? (sqrtD - 3) / 2 : 0;

    /* Adjust r to satisfy r(r+3)/2 >= n and minimal. */
    while ((unsigned __int128)r * (r + 3) / 2 < n)
        ++r;
    while (r > 0 && (unsigned __int128)(r - 1) * (r + 2) / 2 >= n)
        --r;
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t r = triangle_row(n);
    idx_t prev = (unsigned __int128)(r - 1) * (r + 2) / 2;   /* entries before row r */
    idx_t k = n - prev;                                       /* position in row r (1‑based) */

    if (k == 1) {
        return (seq_t)3 * (seq_t)r - (seq_t)1;
    } else if (k == 2) {
        if (r == 1)
            return (seq_t)1;
        else
            return (seq_t)6 * (seq_t)r - (seq_t)6;
    } else {
        idx_t m = r - (k - 1);                               /* >=0 */
        if (m == 0)
            return (seq_t)4;
        else if (m == 1)
            return (seq_t)12;
        else
            return (seq_t)(9 * (seq_t)m + (seq_t)3);
    }
}
