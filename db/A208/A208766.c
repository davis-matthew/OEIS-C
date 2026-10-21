/*
 * A208766  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208765; see the Formula section.
 * https://oeis.org/A208766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangle of coefficients of polynomials v(n,x) from A208765.
    The sequence a(n) is the concatenation of the rows of the triangle
    T(n,k) defined by
          T(n,k) = 2*T(n-1,k) + T(n-1,k-1)
                  – T(n-2,k) – T(n-2,k-1) + 4*T(n-2,k-2)
    with initial rows
          T(1,0) = 1
          T(2,0) = 1 , T(2,1) = 3
    and T(n,k)=0 for k<0 or k>n.
    a(1)=T(1,0), a(2)=T(2,0), a(3)=T(2,1), a(4)=T(3,0), …  */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>   /* size_t */
#include "../utils/get.h"

/* static cache of computed rows */
static seq_t **tri_rows = NULL;   /* index r (≥1) → pointer to array of r values */
static size_t  row_cap  = 0;      /* allocated capacity of tri_rows array   */
static size_t  max_row  = 0;      /* largest row number computed so far     */

/* compute rows up to 'target' (inclusive) on demand */
static void ensure_rows(size_t target)
{
    while (max_row < target) {
        size_t r = max_row + 1;          /* new row number to compute (1‑based) */

        /* resize the storage for row pointers if necessary */
        if (r > row_cap) {
            size_t new_cap = row_cap ? row_cap * 2 : 4;
            if (new_cap < r) new_cap = r;
            seq_t **tmp = (seq_t**)realloc((void*)tri_rows, new_cap * sizeof(seq_t*));
            if (!tmp && new_cap) exit(1);
            tri_rows = tmp;
            row_cap  = new_cap;
        }

        /* allocate the row array */
        seq_t *row = (seq_t*)malloc(r * sizeof(seq_t));
        if (!row) exit(1);

        if (r == 1) {                /* base row 1  : [1]          */
            row[0] = (seq_t)1;
        } else if (r == 2) {         /* base row 2  : [1,3]        */
            row[0] = (seq_t)1;
            row[1] = (seq_t)3;
        } else {                     /* recurrence for r ≥ 3        */
            seq_t *prev1 = tri_rows[r-1];   /* row r‑1 (length r‑1)   */
            seq_t *prev2 = tri_rows[r-2];   /* row r‑2 (length r‑2)   */

            for (size_t k = 0; k < r; ++k) {
                seq_t a = get(prev1, r-1, k);          /* T(r-1,k)   */
                seq_t b = get(prev1, r-1, k-1);        /* T(r-1,k-1) */
                seq_t c = get(prev2, r-2, k);          /* T(r-2,k)   */
                seq_t d = get(prev2, r-2, k-1);        /* T(r-2,k-1) */
                seq_t e = get(prev2, r-2, k-2);        /* T(r-2,k-2) */

                seq_t sum1 = a * (seq_t)2;             /* 2*T(r-1,k) */
                if (k > 0)   sum1 += b;                /* +T(r-1,k-1) */
                if (k >= 2)  sum1 += e * (seq_t)4;     /* +4*T(r-2,k-2) */

                seq_t sum2 = c;                        /* -T(r-2,k)   */
                if (k > 0 && k <= r-2) sum2 += d;     /* -T(r-2,k-1) */

                row[k] = sum1 - sum2;                  /* recurrence */
            }
        }

        tri_rows[r] = row;
        max_row = r;
    }
}

/* public function requested by the framework */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* invalid index, should not happen */

    /* Find the row r that contains index n:
       cumulative sum of 1..r = r*(r+1)/2; we search until it exceeds n. */
    size_t r = 0, cum = 0;
    while (cum < (size_t)n) {
        ++r;
        cum += r;
    }
    size_t prev_sum = cum - r;                 /* sum of 1..(r-1)      */
    size_t k = (size_t)n - prev_sum - 1;       /* column index in row  */

    /* compute all rows up to r if not already done */
    ensure_rows(r);

    return tri_rows[r][k];
}
