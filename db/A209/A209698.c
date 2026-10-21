/*
 * A209698  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209697; see the Formula section.
 * https://oeis.org/A209698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A209698 – triangle of coefficients of v(n,x)
   The array is built from the recurrence
     u(1,x) = 1,        v(1,x) = 1
     u(n,x) = x*u(n-1,x) + x*v(n-1,x)
     v(n,x) = x*u(n-1,x) + (x+1)*v(n-1,x) + 1
   The term a(n) is the coefficient of x^k in v(r,x) where
   r is the row containing the n‑th element in the antidiagonal
   ordering (offset 1).  For n=1,2,… the sequence starts
   1,2,2,3,4,4,4,7,10,8,5,11,20,24,16,…   */

/* Keep enough rows for n up to ≈10000 – r≈141, so 200 is safe */
#include <stddef.h>
#include <stdbool.h>

#define MAXR 200          /* maximum row that can be stored */

static seq_t v_rows[MAXR+1][MAXR+1];    /* triangular array of v‑coefficients   */
static seq_t u_prev[MAXR+1];            /* coefficients of u for the previous row   */
static seq_t v_prev[MAXR+1];            /* coefficients of v for the previous row   */
static size_t max_row = 0;              /* largest row that has been computed        */

/* Ensure that rows up to and including 'r' are computed.  */
static void compute_to_row(size_t r)
{
    if (r > MAXR) return;            /* no support beyond MAXR – n≤10000 is guaranteed */

    if (max_row == 0)                 /* initialise row 1 */
    {
        u_prev[0] = (seq_t)1;
        v_prev[0] = (seq_t)1;
        v_rows[1][0] = (seq_t)1;
        max_row = 1;
    }

    for (size_t row = max_row + 1; row <= r; ++row)
    {
        size_t prev_len = row - 1;     /* previous row length    */
        size_t len      = row;        /* current row length     */

        seq_t u_cur[MAXR+1];
        seq_t v_cur[MAXR+1];

        /* u(row,x) = x*u(prev,x) + x*v(prev,x)  */
        u_cur[0] = (seq_t)0;
        for (size_t k = 1; k < len; ++k) {
            u_cur[k] = u_prev[k-1] + v_prev[k-1];
        }

        /* v(row,x) = x*u(prev,x) + (x+1)*v(prev,x) + 1  */
        v_cur[0] = v_prev[0] + (seq_t)1;
        for (size_t k = 1; k < len; ++k) {
            seq_t sum = u_prev[k-1] + v_prev[k-1];
            if (k < prev_len) sum += v_prev[k];
            v_cur[k] = sum;
        }

        /* Store the new row's v‑coefficients */
        for (size_t k = 0; k < len; ++k) {
            v_rows[row][k] = v_cur[k];
        }

        /* Prepare for next iteration: new previous row */
        for (size_t k = 0; k < len; ++k) {
            u_prev[k] = u_cur[k];
            v_prev[k] = v_cur[k];
        }

        max_row = row;
    }
}

/* Find the row r such that triangular(r-1) < n ≤ triangular(r)   */
static size_t row_of_n(idx_t n)
{
    size_t r = 1;
    idx_t tri = 1;                 /* triangular(r) */
    while (tri < n) {
        ++r;
        tri += (idx_t)r;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; undefined for n=0 */

    size_t r = row_of_n(n);
    compute_to_row(r);                      /* guarantees row r exists    */

    idx_t tri_before = (idx_t)(r - 1) * (idx_t)r / 2;
    size_t col = (size_t)(n - tri_before - 1);   /* column index 0‑based     */

    return v_rows[r][col];
}
