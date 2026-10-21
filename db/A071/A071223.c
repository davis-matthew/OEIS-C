/*
 * A071223  Triangle T(n,k) (n >= 2, 1 <= k <= n) read by rows: number of
 *          linearly inducible orderings of n points in k-dimensional
 *          Euclidean space.
 * https://oeis.org/A071223
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) is the entry T(r,k) of the triangle T(n,k) defined by
 *            T(n,1) = 2, T(n,n-1) = n!, T(n+1,k) = T(n,k) + n · T(n,k-1)
 *            where n ≥ 2 and 1 ≤ k ≤ n-1. a(n) corresponds to the (n‑2)th
 *            term (0‑based) of the triangular array read row by row
 * Range:     u8 n <= 14, u16 n <= 34, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2.  For smaller indices
       the return value is unspecified. */
    if (n < 2)
        return 0;

    /* Convert to a 0‑based index into the triangle:
       m = n − 2 gives the position starting from the first element. */
    idx_t m = n - 2;
    /* Find the row r such that S(r‑1) ≤ m < S(r),
       where S(r) = (r‑1)·r/2 is the cumulative number of terms
       up to row r. */
    idx_t r = 2;
    while ((r - 1) * r / 2 <= m)
        ++r;

    /* Determine the column k (1‑based) within this row. */
    idx_t start_index = 2 + (r - 2) * (r - 1) / 2;   /* first index of row r */
    idx_t k = n - start_index + 1;                   /* 1‑based column */

    /* Special case: the first column is always 2. */
    if (k == 1)
        return 2;

    /* Allocate space for the values of the current row (1..k). */
    size_t sz = (size_t)k + 1;      /* indices 0..k, we ignore 0 */
    seq_t *row = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!row)
        return 0;                   /* unlikely in tests */

    /* Initialise the array with zeros. */
    for (size_t i = 0; i <= k; ++i)
        row[i] = 0;

    /* Row 2: T(2,1) = 2, and the factorial of 2 is also 2. */
    seq_t fact = 1;                 /* factorial of the previous row (1!) */
    for (idx_t cur = 2; cur <= r; ++cur) {
        fact = fact * (seq_t)cur;   /* factorial of current row (cur!) */

        /* Set the factorial entry if it falls within the desired column. */
        if (cur - 1 <= k)
            row[cur - 1] = fact;

        /* Apply the recurrence for k  ≤ cur-2. */
        idx_t maxj = cur - 2;
        if (maxj > k)
            maxj = k;
        for (idx_t j = maxj; j >= 2; --j) {
            row[j] = row[j] + (seq_t)(cur - 1) * row[j - 1];
        }
    }

    seq_t result = row[k];
    free(row);
    return result;
}
