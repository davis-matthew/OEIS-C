/*
 * A039961  Triangle of coefficients in a Fibonacci-like sequence of
 *          polynomials.
 * https://oeis.org/A039961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039961 – Triangle of coefficients in a Fibonacci‑like sequence of polynomials.
 *
 *  Recurrence:    q_{n+2}(x) = x*q_{n+1}(x) - q_{n}(x)
 *                 q_1(x) = q_2(x) = 1
 *
 *  a(n) is the n‑th element of the flattened triangle of coefficients of the
 *  polynomials q_n, listed row‑by‑row with the coefficients of each q_n
 *  given in descending degree order.
 *
 *  Example (first rows):
 *      1
 *      1
 *      1  -1
 *      1  -1  -1
 *      1  -1  -2  1
 *      1  -1  -3   2  1
 *      ...
 */

#include <stdlib.h>
#include <stddef.h>

/* Return the length of row r of the triangle.
 * For r = 1 or 2 the row has length 1.
 * For r >= 3 the row length equals r-1. */
static inline idx_t row_length(idx_t r)
{
    return (r <= 2) ? 1 : r - 1;
}

/* Static cache of the computed rows.
 * coeffs[i] points to an array of coefficients of q_i (descending degree).
 */
static seq_t **coeffs = NULL;
static idx_t max_row   = 0;

/* Compute all rows up to and including row r. */
static void compute_up_to(idx_t r)
{
    if (r <= max_row)
        return;

    /* Resize the coeffs array to hold indices up to r.  The 0‑th entry is unused. */
    size_t new_cap = (size_t)(r + 1);
    void *tmp = realloc(coeffs, new_cap * sizeof(seq_t *));
    coeffs = (seq_t **)tmp;
    if (coeffs == NULL)
        abort();    /* allocation failure – terminate */

    /* Compute rows > max_row up to r. */
    for (idx_t i = max_row + 1; i <= r; ++i)
    {
        idx_t len         = row_length(i);
        size_t sz         = (size_t)len;
        coeffs[i] = (seq_t *)malloc(sz * sizeof(seq_t));
        if (coeffs[i] == NULL)
            abort();

        if (i <= 2)          /* q1 and q2 are the constant polynomial 1 */
        {
            coeffs[i][0] = 1;
            continue;
        }

        /* Indices of the two preceding rows. */
        idx_t len_prev      = row_length(i - 1);
        idx_t len_prevprev  = row_length(i - 2);
        size_t sz_prev      = (size_t)len_prev;
        size_t sz_prevprev  = (size_t)len_prevprev;

        /* Initialize to zero – will hold x * q_{i-1}. */
        for (size_t k = 0; k < sz; ++k)
            coeffs[i][k] = 0;

        /* Copy coefficients of x * q_{i-1}.  Since x*q_{i-1} has length
         * len_prev + 1 (which is len), the constant term is zero. */
        for (size_t k = 0; k < sz_prev; ++k)
            coeffs[i][k] = coeffs[i - 1][k];

        /* Subtract q_{i-2} from the lower-degree part of the result.
         * The alignment offset is len - len_prevprev. */
        size_t offset = sz - sz_prevprev;
        for (size_t k = 0; k < sz_prevprev; ++k)
            coeffs[i][offset + k] -= coeffs[i - 2][k];
    }

    max_row = r;
}

/* Find the row index r such that a(n) lies in row r.  Row r satisfies
 *   1 + r(r-1)/2 >= n  and  1 + (r-1)(r-2)/2 < n .
 */
static idx_t find_row(idx_t n)
{
    idx_t low  = 1;
    idx_t high = 2;
    /* Increase high until the cumulative count exceeds n. */
    while (1 + high * (high - 1) / 2 < n)
        high <<= 1;

    /* Binary search for minimal r with cumulative count >= n. */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if (1 + mid * (mid - 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1 – safe fallback */

    /* Determine the row containing the n-th flattened coefficient. */
    idx_t r = find_row(n);

    /* Ensure that rows up to r have been computed. */
    compute_up_to(r);

    /* Special handling for rows 1 and 2 (single element each). */
    if (r <= 2)
        return coeffs[r][0];

    /* Number of terms preceding row r. */
    idx_t prev = 1 + (r - 1) * (r - 2) / 2;
    /* Zero‑based offset within row r. */
    idx_t offset = n - prev - 1;

    return coeffs[r][offset];
}
