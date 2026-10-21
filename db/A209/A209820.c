/*
 * A209820  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209819; see the Formula section.
 * https://oeis.org/A209820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let u(1,x)=1, v(1,x)=1. For n>1: u(n,x) = x*u(n-1,x) +
 *            2x*v(n-1,x) + 1 v(n,x) = (x+1)*u(n-1,x) + x*v(n-1,x) + 1 A209820
 *            is the triangle of the coefficients of v(n,x), listed
 *            row‑by‑row. The function returns the n‑th element of this
 *            linearised triangle (offset 1)
 * Range:     u8 n <= 25, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static storage for rows of u and v polynomials */
static seq_t **u_rows = NULL;          /* u_rows[k-1] = coeffs of u(k,x) */
static seq_t **v_rows = NULL;          /* v_rows[k-1] = coeffs of v(k,x) */
static size_t rows_allocated = 0;      /* allocated size of the pointers */
static size_t rows_count = 0;          /* number of rows actually computed */

/* enlarge the pointer arrays if necessary, keeping the capacity a power of two */
static void ensure_capacity(size_t need)
{
    if (rows_allocated < need) {
        size_t new_alloc = (rows_allocated == 0) ? 1 : rows_allocated;
        while (new_alloc < need)
            new_alloc <<= 1;
        u_rows = (seq_t **)realloc(u_rows, new_alloc * sizeof(seq_t *));
        v_rows = (seq_t **)realloc(v_rows, new_alloc * sizeof(seq_t *));
        rows_allocated = new_alloc;
    }
}

/* generate rows up to row 'm' (1‑based) */
static void ensure_rows(idx_t m)
{
    if (rows_count >= (size_t)m)
        return;                    /* already have what we need */

    /* grow storage if needed */
    ensure_capacity((size_t)m);

    for (size_t idx = rows_count + 1; idx <= (size_t)m; ++idx) {
        size_t len = idx;                          /* number of coefficients in row idx */
        seq_t *u = (seq_t *)malloc(len * sizeof(seq_t));
        seq_t *v = (seq_t *)malloc(len * sizeof(seq_t));
        u_rows[idx - 1] = u;
        v_rows[idx - 1] = v;

        if (idx == 1) {          /* base case */
            u[0] = (seq_t)1;
            v[0] = (seq_t)1;
        } else {
            size_t prev_len = idx - 1;
            seq_t *prev_u = u_rows[idx - 2];
            seq_t *prev_v = v_rows[idx - 2];

            u[0] = (seq_t)1;            /* constant term of u */
            v[0] = prev_u[0] + (seq_t)1;/* v_0 = u_0 + 1 */

            for (size_t j = 1; j < len; ++j) {
                /* u_next[j] = prev_u[j-1] + 2*prev_v[j-1] */
                seq_t term = prev_u[j - 1];
                term += prev_v[j - 1];
                term += prev_v[j - 1];   /* equivalent to 2*prev_v[j-1] */
                u[j] = term;

                /* v_next[j] = (j<prev_len?prev_u[j]:0) + prev_u[j-1] + prev_v[j-1] */
                seq_t uj = (j < prev_len) ? prev_u[j] : (seq_t)0;
                v[j] = uj + prev_u[j - 1] + prev_v[j - 1];
            }
        }
    }
    rows_count = (size_t)m;
}

/* return the n-th element of the triangle (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n=0, return 0 */

    /* find row containing the n-th element */
    idx_t row = 1;
    idx_t sum = 1;                 /* sum of lengths up to current row */
    while (sum < n) {
        ++row;
        sum += row;
    }
    idx_t prev_sum = sum - row;    /* sum of all previous rows */
    idx_t pos = n - prev_sum - 1;  /* zero‑based position in the row */

    /* generate rows up to 'row' if needed */
    ensure_rows(row);

    /* return coefficient of x^pos in v(row,x) */
    return v_rows[row - 1][pos];
}
