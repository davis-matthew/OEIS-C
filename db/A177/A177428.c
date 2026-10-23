/*
 * A177428  Triangle T(n,m)= A141686(n,m)*(m-1)! read by rows, n>=1, 1<=m<=n.
 * https://oeis.org/A177428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,m) = A008292(n,m) * (n-1)! / (n-m)! read by rows (n≥1,
 *            1≤m≤n)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n‑th term of the row‑by‑row listing of that triangle. */

#include <stddef.h>
#include <stdlib.h>

/* Compute the n‑th term, 1‑based. */
seq_t a(idx_t n)
{
    /* --- Find the row r and column m corresponding to index n --- */
    idx_t r = 1;
    while (((r * (r + 1)) / 2) < n) ++r;            /* r*(r+1)/2 >= n */
    idx_t m = n - ((r - 1) * r) / 2;                 /* 1 ≤ m ≤ r */

    /* --- Cache for Eulerian numbers A008292 --- */
    static seq_t **euler_rows = NULL;               /* 1‑based index, euler_rows[0] unused */
    static idx_t max_row = 0;                       /* largest row that has been built */

    /* Allocate the array of row pointers on first use. */
    if (euler_rows == NULL) {
        euler_rows = (seq_t**)calloc(1, sizeof(seq_t*));  /* size 1, will grow later */
        max_row   = 0;
    }

    /* Extend the cache if a larger row is required. */
    if (r > max_row) {
        /* Increase the array of pointers. */
        euler_rows = (seq_t**)realloc(euler_rows, (r + 1) * sizeof(seq_t*));
        /* Build new rows up to r. */
        for (idx_t i = max_row + 1; i <= r; ++i) {
            euler_rows[i] = (seq_t*)calloc((size_t)i + 1, sizeof(seq_t)); /* 1‑based columns */
            if (i == 1) {
                euler_rows[i][1] = 1;
            } else {
                euler_rows[i][1] = 1;                         /* first column */
                for (idx_t k = 2; k < i; ++k) {               /* interior entries */
                    euler_rows[i][k] =
                        (seq_t)k * euler_rows[i - 1][k] +
                        (seq_t)(i - k + 1) * euler_rows[i - 1][k - 1];
                }
                euler_rows[i][i] = 1;                         /* last column */
            }
        }
        max_row = r;
    }

    /* --- Compute the multiplicative factor (r-1)!/(r-m)! --- */
    seq_t mult = 1;
    idx_t start = r - m + 1;          /* first factor to multiply */
    for (idx_t t = start; t <= r - 1; ++t) {
        mult *= (seq_t)t;
    }

    /* Return the product of the Eulerian number and the factor. */
    return euler_rows[r][m] * mult;
}
