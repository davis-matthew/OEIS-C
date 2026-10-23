/*
 * A141760  Triangle T, read by rows, where the g.f. of column k in matrix
 *          power T^m is given by: 1/(1-x)^m = Sum_{n>=k} [T^m](n,k) *
 *          x^(n-k)/(1+x)^{n(n-1)/2 - k(k-1)/2} for k>=0.
 * https://oeis.org/A141760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 44, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141760
   Triangle T read by rows.
   T(k,k)=1
   T(n,k)=1+Sum_{j=k}^{n-1} (-1)^{n-j+1} * T(j,k) * binomial( j(j-1)/2 - k(k-1)/2 + n-j-1, n-j )
   for n>k.
   a(n) is the element T(r,c) where r*(r+1)/2 <= n < (r+1)(r+2)/2
   and c = n - r*(r+1)/2.  Initial values:
   a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1, …
   (see OEIS for further terms). */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Computes the n‑th term of sequence A141760. */
seq_t a(idx_t n)
{
    /* Static storage for computed rows.  rows[i] is a pointer to a
       dynamically allocated array holding the i‑th row of the triangle. */
    static seq_t **rows = NULL;
    static size_t rows_count = 0;   /* number of rows currently allocated */

    /* Find the zero‑based row r that contains the linear index n. */
    idx_t r = 0;
    while ( (r + 1ULL) * (r + 2ULL) / 2ULL <= n )
        ++r;

    /* Build rows up to r (inclusive) if they are missing. */
    while ( rows_count <= (size_t)r )
    {
        size_t cur = rows_count;                       /* current row number */
        size_t len = cur + 1U;                         /* number of entries in this row */
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) abort();                             /* fatal if allocation fails */

        for (size_t k = 0; k < len; ++k)
        {
            if (k == cur) {                            /* diagonal entry */
                row[k] = (seq_t)1;
                continue;
            }

            /* Compute T(cur,k) by the recurrence.  All operations
               are performed in seq_t type, as required.  Two running
               sums gather even and odd sign contributions to avoid
               negative intermediates. */
            seq_t sum_pos = (seq_t)0;
            seq_t sum_neg = (seq_t)0;

            for (size_t j = k; j < cur; ++j)
            {
                /* Binomial coefficient parameters.  Use idx_t to
                   keep intermediate calculations in an unsigned 64‑bit
                   domain. */
                idx_t part1 = (j * (j - 1ULL) / 2ULL);
                idx_t part2 = (k * (k - 1ULL) / 2ULL);
                idx_t a_val  = part1 - part2 + (cur - j) - 1ULL;
                idx_t b_val  = (cur - j);

                seq_t term = rows[j][k] * binomial(a_val, b_val);

                /* The sign is determined by n-j. */
                if ( ((cur - j) & 1ULL) == 0ULL )   /* even difference → +term */
                    sum_pos += term;
                else                                /* odd difference  → -term */
                    sum_neg += term;
            }

            /* T(cur,k) = 1 + sum_neg - sum_pos.  The final result is
               guaranteed to be non‑negative. */
            row[k] = (seq_t)1 + sum_neg - sum_pos;
        }

        /* Expand the table of row pointers. */
        seq_t **tmp = (seq_t **)realloc(rows, (rows_count + 1) * sizeof(seq_t *));
        if (!tmp) abort();                            /* fatal if reallocation fails */
        rows = tmp;
        rows[rows_count] = row;
        ++rows_count;
    }

    /* Convert the linear index n into row/column coordinates. */
    size_t offset = (size_t)r * (r + 1ULL) / 2ULL;
    size_t idx    = (size_t)(n - offset);

    return rows[r][idx];
}
