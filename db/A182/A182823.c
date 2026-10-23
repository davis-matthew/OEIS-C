/*
 * A182823  Coefficient array of orthogonal polynomials
 *          P(n,x)=(x-n)*P(n-1,x)-(n-1)^2*P(n-2,x), P(0,x)=1, P(1,x)=x-1.
 * https://oeis.org/A182823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, look
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 36, i32 n <= 92, i64 n <= 231,
 *            i128 n <= 596, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A182823
   Coefficient array of orthogonal polynomials
   P(0,x)=1, P(1,x)=x-1,
   P(n,x)=(x-n)P(n-1,x)-(n-1)^2P(n-2,x)
   a(n) = coefficient of x^(n-T_r) in P(r,x),
   where T_r = r*(r+1)/2  (triangular numbers) and r = ⌊(√(8n+1)-1)/2⌋
   Example triangle:
      1,
     -1,  1,
      1, -3,  1,
      …
*/
#include <stdlib.h>
#include <stdint.h>

/* Global cache of polynomial coefficient rows */
static seq_t **rows = NULL;          /* rows[i] points to coefficient array of P(i,x) */
static size_t  rows_capacity = 0;    /* physical size of rows array */
static idx_t   rows_used = 0;        /* number of rows actually computed */

/* Ensure capacity of the rows pointer array, reallocating if necessary */
static void ensure_rows_capacity(size_t needed)
{
    if (rows_capacity < needed) {
        size_t new_cap = rows_capacity ? rows_capacity * 2 : 4;
        if (new_cap < needed) new_cap = needed;
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp) {
            /* out of memory – terminate */
            exit(EXIT_FAILURE);
        }
        rows = tmp;
        rows_capacity = new_cap;
    }
}

/* Position r such that T_r <= n < T_{r+1}
   (T_r = r*(r+1)/2).  For n up to 10^6, linear search is trivial. */
static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    /* Increment r while the next triangular number is still <= n */
    while (((r + 1U) * (r + 2U)) / 2U <= n) {
        ++r;
    }
    return r;
}

/* Build all rows up to and including row 'upto' (inclusive) */
static void build_rows_up_to(idx_t upto)
{
    if (rows_used > upto) return;      /* already available */

    ensure_rows_capacity((size_t)(upto + 1));

    for (idx_t r = rows_used; r <= upto; ++r) {
        /* Allocate coefficient array of length r+1 */
        seq_t *coef = (seq_t *)malloc(((size_t)r + 1U) * sizeof(seq_t));
        if (!coef) exit(EXIT_FAILURE);
        rows[r] = coef;

        if (r == 0) {
            /* P0(x) = 1 */
            coef[0] = (seq_t)1U;
        } else if (r == 1) {
            /* P1(x) = x - 1 */
            coef[0] = (seq_t)(-1);
            coef[1] = (seq_t)1U;
        } else {
            seq_t *prev  = rows[r - 1];   /* P_{r-1} */
            seq_t *prev2 = rows[r - 2];   /* P_{r-2} */

            seq_t r_seq        = (seq_t)r;
            seq_t r_minus_1_sq = (seq_t)(((seq_t)(r - 1U)) * ((seq_t)(r - 1U)));

            for (idx_t k = 0U; k <= r; ++k) {
                seq_t val = (seq_t)0;

                if (k > 0U)      /* contribution from x * P_{r-1} */
                    val += prev[k - 1U];
                if (k < r)       /* subtract n * P_{r-1}[k] */
                    val -= r_seq * prev[k];
                if (k < r - 1U)  /* subtract (n-1)^2 * P_{r-2}[k] */
                    val -= r_minus_1_sq * prev2[k];

                coef[k] = val;
            }
        }
    }

    rows_used = upto + 1U;
}

/* Public entry point – return a(n) */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t t_r = (r * (r + 1U)) / 2U;   /* triangular number T_r */
    idx_t offset = n - t_r;            /* index inside row r */

    if (rows_used <= r) {
        build_rows_up_to(r);
    }

    return rows[r][offset];
}
