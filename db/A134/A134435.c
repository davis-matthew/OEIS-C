/*
 * A134435  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} having k odd entries that are followed by a smaller
 *          entry (n >= 0, k >= 0).
 * https://oeis.org/A134435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattening of the triangle T(n,k) where T(2m,k) =
 *            (m!)^2 * C(m-1,k) * C(m+1,k+1) T(2m+1,k) = m! * (m+1)! * C(m,k)
 *            * C(m+1,k) (0 ≤ k < ceil(n/2))
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   total_terms_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/total_terms_upto.h"

/* --- small helpers ----------------------------------------------------- */
static seq_t fact_upto(idx_t n)
{
    seq_t r = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i)
        r = r * (seq_t)i;
    return r;
}

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* --- main routine ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;          /* row 0 */

    /* binary search for row r such that total_terms_upto(r-1) <= n < total_terms_upto(r) */
    idx_t low = (idx_t)0, high = n + (idx_t)1;
    while (low + (idx_t)1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (total_terms_upto(mid) > n)
            high = mid;
        else
            low = mid;
    }
    idx_t r = high;

    /* index k inside the row */
    idx_t prev = (r == (idx_t)0) ? (idx_t)0 : total_terms_upto(r - (idx_t)1);
    idx_t k = n - prev;

    /* compute T(r,k) */
    if ((r & (idx_t)1) == (idx_t)0) {          /* even row r = 2m */
        idx_t m = r >> 1;
        seq_t fact_m = fact_upto(m);
        seq_t res = fact_m * fact_m;           /* (m!)^2 */
        res = res * binom(m - (idx_t)1, k);
        res = res * binom(m + (idx_t)1, k + (idx_t)1);
        return res;
    } else {                                   /* odd row r = 2m+1 */
        idx_t m = (r - (idx_t)1) >> 1;
        seq_t fact_m  = fact_upto(m);
        seq_t fact_m1 = fact_upto(m + (idx_t)1);
        seq_t res = fact_m * fact_m1;          /* m! * (m+1)! */
        res = res * binom(m, k);
        res = res * binom(m + (idx_t)1, k);
        return res;
    }
}
