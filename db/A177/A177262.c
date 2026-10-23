/*
 * A177262  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} starting with exactly k consecutive integers
 *          (1<=k<=n).
 * https://oeis.org/A177262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r is the smallest integer with r(r+1)/2 ≥
 *            n, k = n – r(r−1)/2, and T(r, k) = (r−k)*(r−k)! if k < r else 1
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 78, u64 n <= 210,
 *            u128 n <= 595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute r(r+1)/2 as seq_t, safe for r up to ~6·10⁹ */
static seq_t _triangular(idx_t r) {
    return ((seq_t)r * (r + 1)) / 2;
}

/* factorial 0! = 1, 1! = 1, 2! = 2, … */
static seq_t _factorial(idx_t m) {
    seq_t f = 1;
    for (idx_t i = 2; i <= m; ++i) {
        f *= (seq_t)i;
    }
    return f;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* just in case, offset is 1 */

    /* find the row r */
    idx_t r = 1;
    while ((idx_t)_triangular(r) < n) ++r;

    /* previous total terms (before this row) */
    idx_t prev = (r > 1) ? (idx_t)_triangular(r - 1) : 0;

    /* column position within the row */
    idx_t k = n - prev;            /* 1 ≤ k ≤ r */

    if (k == r)                    /* last element of the row */
        return 1;

    idx_t t = r - k;               /* t = r - k, 1 ≤ t ≤ r-1 */
    return (seq_t)t * _factorial(t);
}
