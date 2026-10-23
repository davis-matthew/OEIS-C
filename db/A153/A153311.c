/*
 * A153311  Coefficient triangle sequence of a polynomial recursion: p(x,n)=(x
 *          + 1)*(p(x, n - 1) + 3^(n - 2)*(x + x^Floor[n/2] + x^(n - 2))); Row
 *          sums are 2*3^n.
 * https://oeis.org/A153311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, uned, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 26, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   unpack_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* In OEIS notation this implements
   p(x,n) = (x + 1) * ( p(x,n-1) + 3^(n-2) * (x + x^⌊n/2⌋ + x^(n-2) ) )
   and returns the flattened sequence a(n) derived from the coefficient
   triangle of the above recursion.  The triangle is generated on
   demand, so repeated calls to a(n) in any order are supported.  */
/* The code is valid C11 and C++17.  seq_t and idx_t are supplied by
   the build environment. */

#include <stdlib.h>
#include "../utils/unpack_index.h"

#if defined(__cplusplus)
/* In C++ use operator new/delete for arrays. */
#define NEW_ARRAY(TYPE, COUNT) new TYPE[COUNT]
#define FREE_ARRAY(ARR) delete[] ARR
#else
/* In C use malloc/free. */
#define NEW_ARRAY(TYPE, COUNT) ((TYPE*)malloc((COUNT) * sizeof(TYPE)))
#define FREE_ARRAY(ARR) free(ARR)
#endif

/* -------------------------------------------------------------- */
/* Global storage ------------------------------------------------ */
/* rows[i] points to the array of coefficients for row i (length i+1). */
static seq_t **rows          = NULL;
static size_t rows_cap       = 0;
static size_t rows_len       = 0;      /* number of rows currently stored */

/* pow3_val holds 3^(pow3_row-2).  Initially pow3_row==2 so pow3_val==1. */
static seq_t pow3_val = 1;
static idx_t pow3_row = 2;

/* -------------------------------------------------------------- */
/* Helper: initialise the first three known rows.                */
static void init_rows(void)
{
    if (rows) return;              /* already initialised */

    rows_cap = 4;                  /* allow room for growth */
    rows_len = 3;                  /* we store rows 0,1,2 */
    rows = NEW_ARRAY(seq_t*, rows_cap);

    /* Row 0 : [2] */
    seq_t *r0 = NEW_ARRAY(seq_t, 1);
    r0[0] = 2;
    rows[0] = r0;

    /* Row 1 : [3,3] */
    seq_t *r1 = NEW_ARRAY(seq_t, 2);
    r1[0] = 3;
    r1[1] = 3;
    rows[1] = r1;

    /* Row 2 : [2,14,2] */
    seq_t *r2 = NEW_ARRAY(seq_t, 3);
    r2[0] = 2;
    r2[1] = 14;
    r2[2] = 2;
    rows[2] = r2;
}

/* -------------------------------------------------------------- */
/* Ensure that all rows up to and including index r are computed. */
static void ensure_row(idx_t r)
{
    init_rows();

    while (rows_len <= (size_t)r) {
        idx_t new_r = (idx_t)rows_len;          /* index of the new row */
        size_t len_prev = new_r;                /* length of previous row */

        /* Increase pow3_val if needed */
        while (pow3_row < new_r) {
            pow3_val = pow3_val * 3;
            ++pow3_row;
        }

        size_t len = new_r + 1;                 /* length of the new row */

        /* Temporary arrays for computation */
        seq_t *B   = NEW_ARRAY(seq_t, len);
        seq_t *curr = NEW_ARRAY(seq_t, len);

        /* Copy previous row into B */
        seq_t *prev = rows[new_r - 1];
        for (size_t i = 0; i < len_prev; ++i)
            B[i] = prev[i];

        /* Add extra terms contributed by 3^(new_r-2) */
        if (new_r >= 3) {
            seq_t add = pow3_val;
            /* indices 1, floor[new_r/2], new_r-2 */
            B[1] += add;
            B[new_r / 2] += add;
            B[new_r - 2] += add;
        }

        /* Compute result coefficients: T(new_r,k) = B[k] + B[k-1] (k>0) */
        curr[0] = B[0];
        for (size_t k = 1; k < len; ++k)
            curr[k] = B[k] + B[k - 1];

        FREE_ARRAY(B);

        /* Store the new row */
        if (rows_len == rows_cap) {
            rows_cap <<= 1;
#if defined(__cplusplus)
            seq_t **tmp = new seq_t*[rows_cap];
            for (size_t i = 0; i < rows_len; ++i)
                tmp[i] = rows[i];
            delete[] rows;
            rows = tmp;
#else
            rows = (seq_t**)realloc(rows, rows_cap * sizeof(seq_t*));
#endif
        }
        rows[rows_len] = curr;
        ++rows_len;
    }
}

/* -------------------------------------------------------------- */

/* -------------------------------------------------------------- */
/* Public sequence function */
seq_t a(idx_t n)
{
    idx_t r, c;
    unpack_index(n, &r, &c);
    ensure_row(r);
    return rows[r][c];
}
