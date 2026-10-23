/*
 * A137454  Irregular triangle read by rows: coefficients of polynomials p(x,
 *          n) where p(x,n) = x^2*p(x,n-1) + (-x-1)*p(x,n-2) + p(x,n-3).
 * https://oeis.org/A137454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 88, i16 n <= 88, i32 n <= 88, i64 n <= 88, i128 n <= 88,
 *            big any n
 * Helpers:   integer_sqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A137454: Irregular triangle read by rows
 *  p(x,-1)=0; p(x,0)=1; p(x,1)=x^2 - x - 1;
 *  p(x,n)=x^2*p(x,n-1) + (-x-1)*p(x,n-2) + p(x,n-3).
 *
 *  a(n) is obtained by flattening the rows:
 *  row r starts at index r^2 and contains 2r+1 coefficients.
 *  a(n) = coefficient of p(x,r) at power k = n - r^2.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/integer_sqrt_u64.h"

static seq_t **coeffs   = NULL;   /* array of coefficient arrays */
static idx_t   coeffs_size = 0;   /* number of rows already computed */
static idx_t   coeffs_cap  = 0;   /* allocated capacity of coeffs array */

/* ensure that rows 0..r (inclusive) are computed and stored in coeffs */
static void ensure_rows(idx_t r)
{
    if (coeffs_size > r) return;                      /* already available */

    /* grow coeffs array if necessary */
    if (coeffs_cap <= r) {
        idx_t new_cap = coeffs_cap ? coeffs_cap << 1 : 4;
        while (new_cap <= r) new_cap <<= 1;
        seq_t **tmp = (seq_t**)calloc((size_t)new_cap, sizeof(seq_t*));
        if (!tmp) abort();
        if (coeffs) {
            memcpy(tmp, coeffs, (size_t)coeffs_size * sizeof(seq_t*));
            free(coeffs);
        }
        coeffs = tmp;
        coeffs_cap = new_cap;
    }

    /* compute missing rows */
    for (idx_t i = coeffs_size; i <= r; ++i) {
        idx_t len = 2 * i + 1;
        seq_t *arr = (seq_t*)calloc((size_t)len, sizeof(seq_t));
        if (!arr) abort();

        if (i == 0) {
            arr[0] = (seq_t)1;
        } else if (i == 1) {
            arr[0] = (seq_t)-1;
            arr[1] = (seq_t)-1;
            arr[2] = (seq_t) 1;
        } else {
            seq_t *p1 = coeffs[i-1];
            seq_t *p2 = coeffs[i-2];
            seq_t *p3 = (i >= 3) ? coeffs[i-3] : NULL;
            idx_t len1 = 2 * (i-1) + 1;
            idx_t len2 = 2 * (i-2) + 1;
            idx_t len3 = (i >= 3) ? 2 * (i-3) + 1 : 0;

            for (idx_t j = 0; j < len; ++j) {
                seq_t val = (seq_t)0;
                if (j >= 2)           val += p1[j-2];           /* x^2 * p_{i-1} */
                if (j >= 1 && j-1 < len2) val -= p2[j-1];       /* -x * p_{i-2} */
                if (j < len2)         val -= p2[j];              /* -1 * p_{i-2} */
                if (p3 && j < len3)   val += p3[j];              /* + p_{i-3}   */
                arr[j] = val;
            }
        }
        coeffs[i] = arr;
    }
    coeffs_size = r + 1;
}

/* required external function */
seq_t a(idx_t n)
{
    /* row number r = floor(sqrt(n)) */
    idx_t r = integer_sqrt_u64(n);
    idx_t k = n - r * r;                /* position inside row r */

    ensure_rows(r);                     /* make sure the polynomial p(x,r) is computed */

    return coeffs[r][k];
}
