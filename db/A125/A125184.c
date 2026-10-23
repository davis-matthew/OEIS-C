/*
 * A125184  Triangle read by rows: T(n,k) is the coefficient of t^k in the
 *          Stern polynomial B(n,t) (n>=0, k>=0).
 * https://oeis.org/A125184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 6843, u16 n <= 6843, u32 n <= 6843, u64 n <= 6843,
 *            u128 n <= 6843, big any n
 * Helpers:   stern_coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS sequence A125184:
   the flattened triangle consisting of the coefficients of the
   Stern polynomials B(n,t).  */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/stern_coeff.h"

/* ----  store lengths of rows and cumulative counts  ---- */
static idx_t *row_len = NULL;       /* number of coefficients in row i  */
static idx_t *cum_len = NULL;       /* cumulative number of terms up to row i */
static size_t  arrays_len = 0;      /* number of rows currently stored     */
static size_t  arrays_cap = 0;      /* capacity of the two arrays           */

/* enlarge the arrays to at least new_cap rows */
static void grow_arrays(size_t new_cap)
{
    if (new_cap <= arrays_cap) return;
    size_t new_cap2 = arrays_cap ? arrays_cap * 2 : 16;
    while (new_cap2 < new_cap) new_cap2 <<= 1;

    row_len = (idx_t *)realloc(row_len, new_cap2 * sizeof(idx_t));
    cum_len = (idx_t *)realloc(cum_len, new_cap2 * sizeof(idx_t));
    arrays_cap = new_cap2;
}

/* compute the number of terms of row i (for i>=2)  */
static idx_t compute_row_len(idx_t i)
{
    if (i == 0 || i == 1) return 1;
    if ((i & 1) == 0)      /* i = 2m  */
        return row_len[i >> 1] + 1;
    else                  /* i = 2m+1 */
    {
        idx_t a = row_len[(i - 1) >> 1];
        idx_t b = row_len[(i + 1) >> 1];
        return (a > b) ? a : b;
    }
}

/* ensure that arrays contain rows up to the smallest r with
   cum_len[r] > n (so that row r contains index n)          */
static void ensure_arrays(idx_t n)
{
    if (arrays_len == 0)          /* first call: initialise rows 0 and 1 */
    {
        grow_arrays(16);
        /* row 0 : length 1, cumulative 1   */
        row_len[0] = 1;
        cum_len[0] = 1;
        /* row 1 : length 1, cumulative 2   */
        row_len[1] = 1;
        cum_len[1] = 2;
        arrays_len = 2;
    }

    /* grow until we have enough cumulative counts */
    while (cum_len[arrays_len - 1] <= n)
    {
        idx_t i = (idx_t)arrays_len;          /* next row index   */
        idx_t l = compute_row_len(i);
        grow_arrays(arrays_len + 1);
        row_len[arrays_len] = l;
        cum_len[arrays_len] = cum_len[arrays_len - 1] + l;
        arrays_len++;
    }
}

/* main sequence function required by the build */
seq_t a(idx_t n)
{
    ensure_arrays(n);

    /* locate the row containing n */
    size_t r = 0;
    while (r < arrays_len && cum_len[r] <= n)
        ++r;

    idx_t before = (r == 0) ? 0 : cum_len[r - 1];
    idx_t k = n - before;

    return stern_coeff((idx_t)r, k);
}
