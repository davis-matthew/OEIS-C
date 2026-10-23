/*
 * A135929  Triangle read by rows: row n gives coefficients of polynomial
 *          P_n(x)= U_{n}(x,1) + 3 * U_{n-2}(x,1) where U is the Chebyshev
 *          polynomial of the second kind, in order of decreasing exponents.
 * https://oeis.org/A135929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n-th term of the flattened triangle of coefficients
 *            of the polynomials P_n(x) defined by P_0(x) = 1, P_1(x) = x,
 *            P_2(x) = x^2 + 2, P_n(x) = x * P_{n-1}(x) - P_{n-2}(x) for n >=
 *            3. The sequence lists the coefficients of P_n(x) in order of
 *            decreasing exponents. Offsets start at 0
 * Range:     i8 n <= 145, i16 n <= 423, i32 n <= 1409, i64 n <= 5007,
 *            i128 n <= 9489, big any n
 * Helpers:   even_size
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/even_size.h"

/* Main function */
seq_t a(idx_t n)
{
    /*--- 1. Determine the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 ---*/
    idx_t low  = 0;
    idx_t high = 1;
    /* grow high until offset(high) > n */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 off_mid = (unsigned __int128)mid * (mid + 1) / 2;
        if (off_mid <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;                       /* row number                */
    unsigned __int128 off128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t offset = (idx_t)off128;            /* start index of row r      */
    idx_t col    = n - offset;               /* column inside the row     */

    /*--- 2. Static cache of rows (even indices only) ---------------------*/
    static seq_t **rows_even = NULL;  /* array of pointers to rows */
    static size_t rows_cap   = 0;      /* allocated capacity         */
    static idx_t   max_row   = 0;      /* highest row currently stored */

    /* initialize cache on first call */
    if (rows_even == NULL) {
        rows_cap = 1;
        rows_even = (seq_t**)malloc(rows_cap * sizeof(seq_t*));
        if (!rows_even) abort();
        /* base row 0 */
        rows_even[0] = (seq_t*)malloc(sizeof(seq_t));
        if (!rows_even[0]) abort();
        rows_even[0][0] = 1;
        max_row = 0;
    }

    /*--- 3. Compute missing rows up to r ---------------------------------*/
    while (max_row < r) {
        idx_t cur = max_row + 1;

        /* ensure capacity */
        if (rows_cap <= (size_t)cur) {
            rows_cap = rows_cap ? rows_cap * 2 : 1;
            while (rows_cap <= (size_t)cur) rows_cap <<= 1;
            rows_even = (seq_t**)realloc(rows_even, rows_cap * sizeof(seq_t*));
            if (!rows_even) abort();
        }

        size_t sz = even_size(cur);            /* number of even coeffs */
        seq_t *row = (seq_t*)calloc(sz, sizeof(seq_t));
        if (!row) abort();

        if (cur == 0) {
            row[0] = 1;
        } else if (cur == 1) {
            row[0] = 1;
        } else if (cur == 2) {
            row[0] = 1;
            row[1] = 2;
        } else {
            /* recurrence for even coefficients */
            size_t sz_prev  = even_size(cur-1);
            size_t sz_prev2 = even_size(cur-2);
            seq_t *prev  = rows_even[cur-1];
            seq_t *prev2 = rows_even[cur-2];

            row[0] = prev[0];                         /* C[n][0] = C[n-1][0] */
            for (size_t j = 1; j < sz; ++j) {
                seq_t a_val = (j < sz_prev) ? prev[j] : 0;
                seq_t b_val = prev2[j-1];
                row[j] = a_val - b_val;               /* C[n][2j] = a - b */
            }
        }

        rows_even[cur] = row;
        ++max_row;
    }

    /*--- 4. Retrieve the requested coefficient ---------------------------*/
    if (col & 1)           /* odd columns are always zero */
        return 0;
    size_t pos = col >> 1;  /* index of the even coefficient */
    return rows_even[r][pos];
}
