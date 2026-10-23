/*
 * A121552  Triangle read by rows: T(n,k) is the number of deco polyominoes of
 *          height n and area k (n>=1, k>=1). A deco polyomino is a directed
 *          column-convex polyomino in which the height, measured along the
 *          diagonal, is attained only in the last column.
 * https://oeis.org/A121552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   cum_row, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes a(n) for OEIS A121552.
 * The terms are the flattened triangle read by rows whose row‑generating
 * polynomials are
 *   P(1,t)=t
 *   P(n,t)=2 t^n ∏_{j=1}^{n-2} (2+t+…+t^j)   (n≥2).
 * The function returns the nth term of this triangle (1‑based indexing).
 */

#include <stddef.h>                     /* size_t */
#include "../utils/seq_alloc_array.h"    /* seq_alloc_array */
#include "../utils/seq_free_array.h"     /* seq_free_array */
#include "../utils/cum_row.h"

/* Zero a seq_t array of given length. */
static void zero_seq_array(seq_t *a, size_t len)
{
    for (size_t i = 0; i < len; ++i)
        a[i] = (seq_t)0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out of range */

    /* Find row r such that cum_row(r‑1) < n ≤ cum_row(r). */
    idx_t r = 1;
    while (cum_row(r) < n) ++r;

    /* Offset within the row (1‑based). */
    idx_t row_start = cum_row(r - 1);
    idx_t offset    = n - row_start;

    /* Leading zeros. */
    if (offset < r) return (seq_t)0;

    /* Row 1 is a single 1. */
    if (r == 1) return (seq_t)1;

    /* Exponent e in the product part. */
    idx_t e = offset - r;

    /* Max exponent in Q(t)=∏_{j=1}^{r-2}(2+t+…+t^j). */
    size_t max_exp = 0;
    if (r > 2)
        max_exp = (size_t)(r - 2) * (size_t)(r - 1) / 2;

    /* If e exceeds the maximum possible exponent (shouldn't happen), return 0. */
    if (e > max_exp) return (seq_t)0;

    /* Allocate two arrays for dynamic programming. */
    size_t len = max_exp + 1;
    seq_t *dp   = seq_alloc_array(len);
    seq_t *next = seq_alloc_array(len);
    if (!dp || !next) {
        if (dp) seq_free_array(dp);
        if (next) seq_free_array(next);
        return (seq_t)0;
    }

    /* Initialize dp. */
    zero_seq_array(dp, len);
    zero_seq_array(next, len);
    dp[0] = (seq_t)1;

    /* Iterate over each factor j = 1 … r-2. */
    size_t jmax = (size_t)(r - 2);
    for (size_t j = 1; j <= jmax; ++j) {
        zero_seq_array(next, len);
        for (size_t s = 0; s <= max_exp; ++s) {
            /* Constant term 2: add dp[s] twice. */
            next[s] = dp[s];
            next[s] += dp[s];

            /* Terms t^i for i = 1 … j. */
            size_t i_max = (j < s) ? j : s;
            for (size_t i = 1; i <= i_max; ++i)
                next[s] += dp[s - i];
        }
        /* Swap dp and next. */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    /* Coefficient of t^e in Q(t) is dp[e]. */
    seq_t coeff = dp[e];
    /* Multiply by 2 according to P(n,t)=2 t^n Q(t). */
    seq_t result = coeff;
    result += coeff;

    /* Clean up. */
    seq_free_array(dp);
    seq_free_array(next);
    return result;
}
