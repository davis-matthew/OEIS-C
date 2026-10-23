/*
 * A177974  Triangle T(n,k) = n!/(n - k)! + n!/k! - n! read by rows, 0 <= k <=
 *          n.
 * https://oeis.org/A177974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   Triangular array T(n,k) = n!/(n-k)! + n!/k! - n! (0 ≤ k ≤ n) The
 *            sequence a(n) is obtained by reading T row‑wise. For index n,
 *            let r be the row number (0‑based) and k = n – r(r+1)/2. Then
 *            a(n) = T(r,k)
 * Range:     i8 n <= 22, i16 n <= 37, i32 n <= 92, i64 n <= 232,
 *            i128 n <= 596, big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"   /* factorial(idx_t n) */

static inline idx_t row_from_index(idx_t n)
{
    /* Find largest r such that r(r+1)/2 <= n. */
    idx_t r = 0;
    /* Since n is small for practical n (≤ 1e6 for 64‑bit tests),
       the following loop is safe and fast. */
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = row_from_index(n);
    idx_t k = n - r * (r + 1) / 2;

    /* Corner cases where the value is trivially small. */
    if (k == 0 || k == r)
        return (seq_t)1;

    if (k == 1 || k == r - 1)
        return (seq_t)r;          /* T(r,1) = T(r,r-1) = r */

    /* General case: compute using multiplicative updates. */
    seq_t fact_r = factorial(r);     /* r!   */

    seq_t A = (seq_t)1;              /* product for n!/(n-k)!  */
    seq_t B = fact_r;                /* product for n!/k!       */

    for (idx_t i = 0; i < k; ++i) {
        A = A * (seq_t)(r - i);      /* multiply by (r-i)      */
        B = B / (seq_t)(i + 1);      /* divide by (i+1)        */
    }

    return A + B - fact_r;           /* T(r,k) = A + B - r! */
}
