/*
 * A209694  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209693; see the Formula section.
 * https://oeis.org/A209694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A209694 – Triangle of coefficients of v(n,x) (jointly generated with A209693) */
/* Recurrence: */
/*  u(1,x)=1, v(1,x)=1 */
/*  u(n,x)=x*u(n-1,x)+x*v(n-1,x) */
/*  v(n,x)=u(n-1,x)+(x+1)*v(n-1,x)+1 */
/* The sequence a(n) lists the coefficients of v(n,x) row by row, left to right. */

#include <stdlib.h>
#include <stdint.h>

static seq_t **u = NULL;          /* coefficient arrays for u(n,x)  – 1‑based rows, 0‑based cols  */
static seq_t **v = NULL;          /* coefficient arrays for v(n,x)  – same layout          */
static size_t  max_rows = 0;      /* highest row computed so far                         */

/* Return the n‑th term of the flattened coefficient triangle */
seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* Find the row r such that r*(r+1)/2 ≥ n (and (r−1)*r/2 < n) */
    size_t r = 1;
    size_t tri = 1;                     /* r*(r+1)/2 for current r */
    while (tri < (size_t)n) {
        ++r;
        tri = r * (r + 1) / 2;
    }

    /* Compute any missing rows up to r */
    if (r > max_rows) {
        size_t new_rows = r;
        u = (seq_t**)realloc(u, new_rows * sizeof(seq_t*));
        v = (seq_t**)realloc(v, new_rows * sizeof(seq_t*));

        for (size_t i = max_rows + 1; i <= new_rows; ++i) {
            size_t len = i;                     /* number of coefficients in row i */
            u[i-1] = (seq_t*)malloc(len * sizeof(seq_t));
            v[i-1] = (seq_t*)malloc(len * sizeof(seq_t));

            if (i == 1) {                       /* base case u(1)=v(1)=1 */
                u[0][0] = 1;
                v[0][0] = 1;
            } else {
                size_t j = i - 1;               /* previous row index (j = i−1) */
                /* u(i,0) has no constant term */
                u[i-1][0] = 0;
                /* v(i,0) = u(j,0)+v(j,0)+1 */
                v[i-1][0] = u[j-1][0] + v[j-1][0] + 1;

                for (size_t k = 1; k < len; ++k) {
                    /* u(i,k) = u(j,k-1)+v(j,k-1) */
                    u[i-1][k] = u[j-1][k-1] + v[j-1][k-1];

                    /* v(i,k) = (u(j,k) if k<j)+(v(j,k) if k<j)+v(j,k-1) */
                    seq_t ujk = (k < j) ? u[j-1][k] : 0;
                    seq_t vjk = (k < j) ? v[j-1][k] : 0;
                    v[i-1][k] = ujk + vjk + v[j-1][k-1];
                }
            }
        }
        max_rows = new_rows;
    }

    /* Compute index k inside row r (0‑based) */
    size_t prev_total = (r - 1) * r / 2;         /* terms before row r */
    size_t k = (size_t)n - prev_total - 1;       /* 0‑based column in row r */

    return v[r-1][k];
}
