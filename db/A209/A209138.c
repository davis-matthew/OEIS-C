/*
 * A209138  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209137; see the Formula section.
 * https://oeis.org/A209138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = linearization of the triangle T(n,k) defined by * T(1,0)
 *            = 1, * T(2,0) = 1, T(2,1) = 2, * T(n,k) = T(n-1,k) + T(n-1,k-1)
 *            + T(n-2,k) + T(n-2,k-1) + T(n-2,k-2) * for 1 < n and 0 ≤ k < n.
 *            a(n) is the n‑th element of the triangle * read row by row.
 *            (Off‑by‑one offset is 1.)
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* triangular number: r*(r+1)/2 */
static idx_t triangular(idx_t r)
{
    return r * (r + 1) / 2;
}

/* find the smallest row r such that triangular(r) >= n */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = 1;
    while (triangular(high) < n) {
        if (high > n)   /* will certainly cover n now */
            break;
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* cache of computed rows */
static seq_t **rows = NULL;      /* rows[1] … rows[maxRow] */
static idx_t capacity = 0;       /* number of row pointers allocated (rows[0] unused) */
static idx_t maxRow = 0;         /* highest row that has been computed */

/* Ensure rows up to target are computed */
static void ensure_rows(idx_t target)
{
    /* grow the row pointer array if necessary */
    if (capacity < target + 1) {
        rows = (seq_t **)realloc(rows, (target + 1) * sizeof(seq_t *));
        for (idx_t i = capacity; i <= target; ++i)
            rows[i] = NULL;
        capacity = target + 1;
    }

    /* compute any missing rows */
    for (idx_t i = maxRow + 1; i <= target; ++i) {
        rows[i] = (seq_t *)malloc((size_t)i * sizeof(seq_t));
        for (idx_t k = 0; k < i; ++k) {
            seq_t val = 0;
            if (i == 1) {                     /* row 1 */
                val = 1;
            } else if (i == 2) {               /* row 2 */
                val = (k == 0) ? 1 : 2;
            } else {                           /* recurrence */
                if (k < i-1)                    /* T(i-1,k)  */
                    val += rows[i-1][k];
                if (k > 0)                      /* T(i-1,k-1) */
                    val += rows[i-1][k-1];
                if (k < i-2)                    /* T(i-2,k)  */
                    val += rows[i-2][k];
                if (k > 0 && k < i-1)           /* T(i-2,k-1) */
                    val += rows[i-2][k-1];
                if (k > 1)                      /* T(i-2,k-2) */
                    val += rows[i-2][k-2];
            }
            rows[i][k] = val;
        }
    }
    maxRow = target;
}

/* Main sequence entry point */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t r = find_row(n);                     /* row index (1‑based) */
    ensure_rows(r);                            /* guarantee the row exists */

    idx_t prev = triangular(r - 1);            /* terms before row r */
    idx_t offset = n - prev - 1;               /* 0‑based column within row r */
    return rows[r][(size_t)offset];
}
