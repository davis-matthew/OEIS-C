/*
 * A139158  Triangle a(n,k) of the expansion coefficients of the Hermite
 *          polynomial 2*H(n/2,x) if n even, of H((n-1)/2,x)+H((n+1)/2,x) if n
 *          odd.
 * https://oeis.org/A139158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   (-1)^j * m! / (j! k!) * 2^k
 * Range:     i8 n <= 31, i16 n <= 68, i32 n <= 68, i64 n <= 68, i128 n <= 68,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 68 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A139158: coefficients of
    2·H_{n/2}(x) for even rows,
    H_{(n−1)/2}(x)+H_{(n+1)/2}(x) for odd rows
    (H_m is the physicists' Hermite polynomial).
    The sequence is flattened row by row.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* locate row r and column c for the flattened index n
   r = row number (starting at 0), c = column within that row */
static void locate_row_col(idx_t n, idx_t *r_out, idx_t *c_out)
{
    idx_t r = 0;
    idx_t used = 0;                       /* total elements before row r */

    while (1) {
        /* length of row r: floor((r+1)/2)+1 = (r>>1)+1+(r&1) */
        idx_t len = (r >> 1) + 1 + (r & 1);

        if (used + len > n) {             /* our element lies in this row */
            *r_out = r;
            *c_out = n - used;
            return;
        }
        used += len;
        ++r;
    }
}

/* coefficient of x^k in the physicists' Hermite polynomial H_m(x) */
static seq_t hermite_coeff(idx_t m, idx_t k)
{
    if (k > m)
        return 0;                         /* outside degree range */

    idx_t delta = m - k;
    if (delta & 1)                         /* m−k must be even */
        return 0;

    idx_t j = delta >> 1;                  /* j = (m−k)/2 */

        seq_t val = binomial(m, k);            /* m! / (k!(m−k)!)  */
    val = val * factorial(delta);         /* multiply by (m−k)! */
    val = val / factorial(j);             /* divide by j!   */
    val = val * pow2(k);                   /* multiply by 2^k */
    if (j & 1)                             /* sign flip if j is odd */
        val = -val;
    return val;
}

/* required function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    idx_t r, c;
    locate_row_col(n, &r, &c);

    seq_t result;
    if ((r & 1) == 0) {                   /* even row: 2·H_{r/2}(x) */
        idx_t m = r >> 1;
        result = hermite_coeff(m, c) * (seq_t)2;
    } else {                               /* odd row: H_{(r−1)/2}(x)+H_{(r+1)/2}(x) */
        idx_t m = (r - 1) >> 1;
        result = hermite_coeff(m, c) + hermite_coeff(m + 1, c);
    }
    return result;
}
