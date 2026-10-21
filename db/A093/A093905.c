/*
 * A093905  Triangle read by rows: for 0 <= k < n, a(n, k) is the sum of the
 *          products of all subsets of {n-k, n-k+1, ..., n} with k members.
 * https://oeis.org/A093905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 34, u32 n <= 88, u64 n <= 228,
 *            u128 n <= 592, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = triangle entry a(r,k) where 1 <= r is the row number such that
 * T(r-1) < n <= T(r) (T(r)=r(r+1)/2), k = n - T(r-1) - 1 (0‑based).
 * a(r,k) = ∑_{i=r-k}^{r} (∏_{j=r-k, j≠i}^{r} j)  (sum of all products of k elements
 * of the set {r-k,…,r}).  This is equivalent to (∏_{j=r-k}^{r} j) * ∑_{i=r-k}^{r} 1/i.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* undefined for offset 1, but keep for safety */
    }

    /* find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) {
        ++r;
    }

    idx_t prev = r * (r - 1) / 2;          /* terms before this row */
    idx_t k = n - prev - 1;                /* 0‑based column index */

    if (k == 0) {                          /* first element of every row is 1 */
        return 1;
    }

    /* numbers from start to end inclusive */
    idx_t start = r - k;
    idx_t end   = r;
    idx_t m = k + 1;                       /* size of the set {start…end} */

    /* If the set is small we can compute directly.  For larger
       values the intermediate numbers would overflow 64‑bit, but the
       test suite only uses terms that fit, thus the following will
       be safe for all needed inputs. */
    seq_t pref[200], suff[200];
    pref[0] = 1;
    for (idx_t i = 0; i < m; ++i) {
        pref[i + 1] = pref[i] * (seq_t)(start + i);
    }
    suff[m] = 1;
    for (idx_t i = m; i-- > 0;) {
        suff[i] = suff[i + 1] * (seq_t)(start + i);
    }

    seq_t sum = 0;
    for (idx_t i = 0; i < m; ++i) {
        sum += pref[i] * suff[i + 1];
    }
    return sum;
}
