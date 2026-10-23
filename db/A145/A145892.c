/*
 * A145892  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} having k adjacent pairs of the form (even,even)
 *          (0<=k<=floor(n/2)-1).
 * https://oeis.org/A145892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where T(r,k) is defined by the triangle of
 *            A145892. For an even row r = 2m: T(2m,k) = (m!)^2 * C(m-1,k) *
 *            C(m+1,k+1) (0 ≤ k ≤ m-1) For an odd row r = 2m+1: T(2m+1,k) = m!
 *            · (m+1)! * C(m-1,k) * C(m+2,k+2) (0 ≤ k ≤ m-1)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */

/* ---------- local helper: factorial ------------------------------------ */
static seq_t my_factorial(idx_t n)
{
    seq_t r = 1;
    for (idx_t i = 2; i <= n; ++i)
        r *= (seq_t)i;
    return r;
}

/* ---------- local helper: binomial coefficient ------------------------- */
static seq_t my_binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;   /* take advantage of symmetry */
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r *= (seq_t)(n - k + i);   /* multiply by numerator */
        r /= (seq_t)i;             /* divide by denominator */
    }
    return r;
}

/* ---------- main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    /* special first two rows */
    if (n == 0) return 1;
    if (n == 1) return 1;

    /* find the row r and offset within that row (k) */
    idx_t cum = 2;   /* entries up to row 1 */
    idx_t r   = 2;
    for (;;) {
        idx_t entries = r >> 1;          /* floor(r/2) */
        if (cum + entries > n) break;    /* row r contains the index */
        cum += entries;
        ++r;
    }
    idx_t offset = n - cum;   /* k = offset within row r */

    if (r & 1) {   /* odd row: r = 2m + 1 */
        idx_t m = (r - 1) >> 1;
        seq_t res = my_factorial(m);
        res *= my_factorial(m + 1);
        res *= my_binomial(m - 1, offset);
        res *= my_binomial(m + 2, offset + 2);
        return res;
    } else {       /* even row: r = 2m */
        idx_t m = r >> 1;
        seq_t res = my_factorial(m);
        res *= my_factorial(m);
        res *= my_binomial(m - 1, offset);
        res *= my_binomial(m + 1, offset + 1);
        return res;
    }
}
