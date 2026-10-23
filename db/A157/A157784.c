/*
 * A157784  Triangle read by rows: the coefficient [x^k] of the polynomial
 *          Product_{i=1..n} (4^(i-1)-x), in row n and column 0 <= k <= n.
 * https://oeis.org/A157784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = coefficient [x^k] of ∏_{i=1}^{r} (4^{i-1} – x), where r
 *            satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2 and k = n – r(r+1)/2. (Row
 *            r, column k of the triangle)
 * Range:     i8 n <= 9, i16 n <= 14, i32 n <= 27, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Find the row r such that r(r+1)/2 ≤ idx < (r+1)(r+2)/2. */
static idx_t
find_row(idx_t idx)
{
    idx_t r = 0;
    /* Increment r until the triangular number exceeds idx. */
    while ((unsigned __int128)r * (r + 1) / 2 <= idx)
        ++r;
    /* r is now one greater than required. */
    return r - 1;
}

seq_t
a(idx_t n)
{
    /* Determine row (r) and column (k) for the flattened index n. */
    idx_t r = find_row(n);
    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri;

    /* Allocate array for the coefficients of row r. */
    /* For arbitrary‑precision seq_t use a cast of malloc. */
    seq_t *cur = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
    if (!cur)
        return (seq_t)0;  /* unlikely; graceful failure */

    /* Base case: row 0. */
    cur[0] = (seq_t)1;

    /* Current power of 4: 4^{i-1} for the factor in step i. */
    seq_t c = (seq_t)1;

    /* Build rows up to r in place.  Update coefficients in reverse
       order so that old values are preserved. */
    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t j = i; ; --j) {
            seq_t a_prev = (j < i ? cur[j] : (seq_t)0);
            seq_t a_prev_mem = (j > 0 ? cur[j - 1] : (seq_t)0);
            cur[j] = a_prev * c - a_prev_mem;
            if (j == 0)
                break;
        }
        c *= (seq_t)4;  /* next power of 4 for the following row */
    }

    seq_t result = cur[k];
    free(cur);
    return result;
}
