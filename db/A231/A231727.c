/*
 * A231727  Triangular array read by rows: row n shows the coefficients of the
 *          polynomial u(n) = c(0) + c(1)*x + ... + c(n)*x^n which is the
 *          denominator of the n-th convergent of the continued fraction [k,
 *          k, k, ... ], where k = (x + 1)/(x - 1).
 * https://oeis.org/A231727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 56, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   mul_x_minus1_sq, mul_x_plus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_x_minus1_sq.h"
#include "../utils/mul_x_plus_one.h"

/* Sequence A231727
   The coefficients are read row by row from the polynomials
   u_n(x) = (x-1)*v_n(x)
   where the auxiliary polynomials v_n satisfy
       v_1(x) = 1
       v_2(x) = x + 1
       v_n(x) = (x+1)*v_{n-1}(x) + (x-1)^2 * v_{n-2}(x)   (n ≥ 3)
   The k‑th term a(k) is the coefficient of x^c in u_r(x),
   where (r,c) are the row and column in the triangular array
   (rows are numbered from 1, columns are numbered from 0).
   The first few rows are:
       -1   1
      -1   0   1
      -2   2  -2   2
      -3   2   0  -2   3
      -5   5  -6   6  -5   5
   and this code reproduces all further terms. */

static seq_t** v_cache = NULL;     /* cache of v_n polynomials, one per row */
static size_t  v_cache_size = 0;   /* number of cached rows */

/* return a pointer to the coefficients of v_r (length r)
   The coefficients are stored in ascending order of degree. */
static const seq_t* get_v(size_t r)
{
    if (r == 0) return NULL;
    if (r > v_cache_size) {
        size_t new_size = r;
        seq_t** new_cache = (seq_t**)realloc(v_cache, new_size * sizeof(seq_t*));
        if (!new_cache) return NULL;      /* allocation failure */
        v_cache = new_cache;
        /* allocate new rows */
        for (size_t i = v_cache_size; i < new_size; ++i) {
            v_cache[i] = (seq_t*)calloc(i + 1, sizeof(seq_t));
            if (!v_cache[i]) return NULL;  /* allocation failure */
        }
        /* initialise first two rows */
        if (new_size >= 1) v_cache[0][0] = (seq_t)1;      /* v_1 = 1 */
        if (new_size >= 2) {
            v_cache[1][0] = (seq_t)1; /* v_2 = x + 1 */
            v_cache[1][1] = (seq_t)1;
        }
        /* compute remaining rows */
        for (size_t i = v_cache_size >= 2 ? v_cache_size : 2; i < new_size; ++i) {
            size_t len   = i + 1;           /* length of v_{i+1} */
            size_t len1  = i;               /* length of v_i     */
            size_t len2  = i - 1;           /* length of v_{i-1} */

            seq_t* v_prev      = v_cache[i - 1];
            seq_t* v_prevprev  = v_cache[i - 2];
            seq_t* t1 = (seq_t*)calloc(len, sizeof(seq_t));
            seq_t* t2 = (seq_t*)calloc(len, sizeof(seq_t));
            if (!t1 || !t2) { free(t1); free(t2); return NULL; }

            mul_x_plus_one(v_prev,     len1, t1);
            mul_x_minus1_sq(v_prevprev, len2, t2);
            for (size_t j = 0; j < len; ++j) v_cache[i][j] = t1[j] + t2[j];

            free(t1); free(t2);
        }
        v_cache_size = new_size;
    }
    return v_cache[r - 1];
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; behaviour for n<1 is undefined */

    /* determine row r and column c from index n */
    size_t r = 1;
    size_t sum = 2;   /* total terms up to row 1 */
    while (sum < (size_t)n) {            /* (r+1)-term rows */
        ++r;
        sum += r + 1;
    }
    size_t offset_prev = ((r - 1) * (r + 2)) / 2;   /* terms before row r */
    size_t c = (size_t)n - offset_prev - 1;         /* column in row r (0‑based) */

    /* obtain v_r */
    const seq_t *v = get_v(r);
    if (!v) return 0; /* allocation failure */

    /* u_r(x) = (x-1)*v_r(x) */
    /* coefficient formula:
           if c == 0 :  -v[0]
           else      :  v[c-1] - v[c]   (v[c] treated as 0 if c > r-1) */
    seq_t coeff;
    if (c == 0) {
        coeff = -(v[0]);
    } else {
        seq_t left  = v[c - 1];
        seq_t right = (c <= r - 1) ? v[c] : (seq_t)0;
        coeff = left - right;
    }
    return coeff;
}
