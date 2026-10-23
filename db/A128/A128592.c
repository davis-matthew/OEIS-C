/*
 * A128592  Triangle, read by rows, of coefficients of q^(nk+k) in the
 *          q-analog of the odd double factorials: T(n,k) = [q^(nk+k)]
 *          Product_{j=1..n+1} (1-q^(2j-1))/(1-q) for n>0, with T(0,0)=1.
 * https://oeis.org/A128592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle read row-wise of the coefficients * of
 *            q^(nk+k) in the q-analog of the odd double factorials: * T(n,k)
 *            = [q^(nk+k)] ∏_{j=1..n+1} (1-q^(2j-1))/(1-q) * with T(0,0)=1.
 *            a(n) = T(r,p) where r is the row containing n * and p = n -
 *            r*(r+1)/2. (OEIS A128592)
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* maximum row that can be produced with the arrays below.
 * For n <= 10000 the row is < 140, so 200 provides a comfortable margin. */
#define MAX_ROW   200
#define MAX_POLY  20000   /* sufficient for MAX_ROW */

/* static state for memoisation */
static idx_t   last_row  = 0;          /* highest row already computed */
static int     cur_len   = 0;          /* current polynomial length   */
static seq_t   poly[MAX_POLY];         /* current polynomial coeffs   */
static seq_t   tmp_poly[MAX_POLY];     /* temporary polynomial         */
static seq_t   triangle[MAX_ROW+1][MAX_ROW+1];

/* ------------------------------------------------------------------ */
/* Determine the row r that contains the linear index n
 * (small n: loop is fast; no overflow for n up to 9e18).           */
static idx_t
row_of(idx_t n)
{
    idx_t r = 0;
    while (1)
    {
        idx_t next = (r+1)*(r+2)/2;
        if (next > n) break;
        ++r;
    }
    return r;
}

/* ------------------------------------------------------------------ */
/* Build rows up to and including target (target >= last_row).   */
static void
precompute_to(idx_t target)
{
    while (last_row < target)
    {
        /* target row = last_row + 1 to compute next */
        idx_t n = last_row + 1;                  /* new row number  */
        /* length of the new factor to multiply by */
        /* factor for j = n+1: 1 + q + ... + q^(2(n+1)-2)  (length = 2n+1)  */
        idx_t factor_len = 2*n + 1;
        idx_t new_len = cur_len + factor_len - 1; /* new polynomial length */

        /* convolution with all‑ones factor of length factor_len */
        for (idx_t j = 0; j < new_len; ++j)
        {
            idx_t a = (j >= factor_len - 1) ? j - (factor_len - 1) : 0;
            idx_t b = (j < (idx_t)cur_len) ? j : (idx_t)cur_len - 1;

            seq_t sum = 0;
            for (idx_t i = a; i <= b; ++i)
                sum += poly[i];
            tmp_poly[j] = sum;
        }

        /* copy back to poly and update current length */
        for (idx_t j = 0; j < new_len; ++j)
            poly[j] = tmp_poly[j];
        cur_len = (int)new_len;

        /* store the table entries for this row */
        /* T(n,k) = coeff at exponent k*(n+1) */
        for (idx_t k = 0; k <= n; ++k)
            triangle[last_row + 1][k] = poly[k * (n + 1)];

        ++last_row;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function requested by the build.                     */
seq_t a(idx_t n)
{
    /* initialise on first call */
    static int initialized = 0;
    if (!initialized)
    {
        poly[0] = 1;
        cur_len = 1;
        triangle[0][0] = 1;
        last_row = 0;
        initialized = 1;
    }

    idx_t r = row_of(n);
    precompute_to(r);

    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;
    return triangle[r][k];
}
