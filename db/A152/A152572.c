/*
 * A152572  Triangle T(n,k) read by rows: T(n,n) = -1, T(n,0) = 5^(n - 1),
 *          T(n,k) = -5^(n - k - 1), 1 <= k <= n - 1.
 * https://oeis.org/A152572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is obtained by flattening the triangle T(r,k) row‑by‑row.
 *            For r ≥ 1: T(r,0) = 5^(r‑1) T(r,k) = -5^(r‑k‑1) for 1 ≤ k ≤ r‑1
 *            T(r,r) = -1 For r = 0 the only entry is T(0,0) = -1. The index n
 *            determines the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. The
 *            value at position n is T(r, j) where j =...
 * Range:     i8 n <= 14, i16 n <= 35, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 5^exp as a seq_t.  No external helpers are required.  */
static seq_t pow5(idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)5;
    return result;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Handle the first row specially (row 0). */
    if (n == 0)
        return -(seq_t)1;

    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2
       using a binary search to avoid floating point arithmetic. */
    idx_t low  = 0;
    idx_t high = n;                 /* r will never exceed n */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;   /* try upper half first */
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Offset of the start of this row in the linearized array. */
    unsigned __int128 offset128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t offset = (idx_t)offset128;   /* fits in idx_t because offset ≤ n */
    idx_t j = n - offset;              /* column index within the row */

    /* Determine the triangle entry. */
    if (j == 0)               /* first element of the row */
        return pow5(r - 1);

    if (j == r)               /* last element of the row */
        return -(seq_t)1;

    /* middle elements */
    idx_t exp = r - j - 1;
    return -(seq_t)pow5(exp);
}
