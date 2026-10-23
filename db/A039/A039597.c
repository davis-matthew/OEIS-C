/*
 * A039597  Triangle read by rows: T(n,k) = number of 2 X inf arrays [ n, n1,
 *          n2, ...; k, k1, k2,... ] with n>=n1>n2>...>=0, k>=k1>k2...>=0,
 *          n>k, n1>k1, ...; n >= 1, k >= 0. Note that once ni or ki = 0, the
 *          strict inequalities become equalities (constant 0 thereafter).
 * https://oeis.org/A039597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 34, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A039597:  Triangle read by rows
 *
 *  T(n,1) = 2^n          (leftmost element)
 *  T(n,n) = C(2n,n)      (rightmost element)
 *  For 1 < k < n:  T(n,k) = T(n,k-1) + T(n-1,k)
 *
 *  a(n) is the flattened sequence, i.e. the elements of the triangle
 *  read row by row.  a(0) = 2 (first element of row 1).
 */

#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/binom.h"

static seq_t **rows = NULL;   /* rows[1] … rows[max_row] (1‑based) */
static size_t max_row = 0;    /* highest row computed so far */

/* Ensure rows 1 … r exist; compute new rows as needed. */
static void ensure_rows(size_t r)
{
    if (r <= max_row)
        return;

    size_t new_size = r + 1;                     /* 1‑based indexing, index 0 unused */
    seq_t **tmp = (seq_t**)realloc(rows, new_size * sizeof(seq_t*));
    if (!tmp) exit(EXIT_FAILURE);
    rows = tmp;

    /* Initialise new rows to NULL */
    for (size_t i = max_row + 1; i <= r; ++i)
        rows[i] = NULL;

    /* Compute new rows */
    for (size_t i = max_row + 1; i <= r; ++i)
    {
        /* Allocate array of seq_t for row i (length = i) */
        seq_t *row = (seq_t*)malloc(sizeof(seq_t) * i);
        if (!row) exit(EXIT_FAILURE);

        /* Leftmost element */
        row[0] = pow2((idx_t)i);

        /* Interior elements (k = 1 … i-2) */
        for (size_t k = 1; k < i - 1; ++k)
            row[k] = rows[i - 1][k] + row[k - 1];

        /* Rightmost element */
        row[i - 1] = binom(2 * (idx_t)i, (idx_t)i);

        rows[i] = row;
    }

    max_row = r;
}

/* Return the n-th element (0‑based) of the flattened triangle. */
seq_t a(idx_t n)
{
    /* Find the row r such that S(r-1) <= n < S(r) where S(r)=r(r+1)/2. */
    size_t r = 1;
    while ( ( (idx_t)r * (r + 1) ) / 2 <= n )
        ++r;

    size_t offset = (size_t)n - ( (r - 1) * r ) / 2;
    ensure_rows(r);
    return rows[r][offset];
}
