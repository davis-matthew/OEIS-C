/*
 * A086610  Triangle of coefficients, read by rows, where T(n,k) is the
 *          coefficient of x^n*y^k in f(x,y) that satisfies f(x,y) = 1/(1-x) -
 *          x^2/(1-x)^2 + xy*f(x,y)^2.
 * https://oeis.org/A086610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the coefficient of x^n y^k in * f(x,y) = 1/(1-x) -
 *            x²/(1-x)² + x y f(x,y)², * read row by row. The triangle T(n,k)
 *            satisfies * T(0,0)=1, T(1,0)=1, T(n,0)=2-n (n≥2), and *
 *            T(n,k)=∑_{a=0}^{n-1}∑_{b=0}^{k-1} T(a,b)·T(n-1-a,k-1-b) (k>0)
 * Range:     i8 n <= 24, i16 n <= 62, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is obtained by flattening T row‑by‑row. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* seq_t and idx_t are defined externally. */
seq_t a(idx_t n)
{
    static seq_t **rows = NULL;   /* rows[i][k] = T(i,k) */
    static size_t row_count = 0;   /* number of rows currently stored */

    /* --------------------------------------------------------------------
       Determine the row r that contains index n.
       The starting index of row r is r(r+1)/2.  We first estimate r with
       the inverse triangular number formula and then correct round‑off.
     -------------------------------------------------------------------- */
    idx_t r = (idx_t)((sqrtl((long double)(8.0L*n + 1.0L)) - 1.0L) / 2.0L);
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n)    --r;

    /* --------------------------------------------------------------------
       Grow the triangle until row r is available.  Each new row i is
       computed from the recurrence; rows[i] has i+1 columns.
     -------------------------------------------------------------------- */
    if (row_count <= (size_t)r) {
        size_t old = row_count;
        row_count = (size_t)r + 1;
        rows = (seq_t**)realloc(rows, row_count * sizeof(seq_t*));
        for (size_t i = old; i < row_count; ++i) {
            rows[i] = (seq_t*)calloc(i + 1, sizeof(seq_t));
            for (size_t k = 0; k <= i; ++k) {
                seq_t val = 0;

                /* ----------------------------------------------------------------
                   Inhomogeneous part: 1/(1-x) gives +1 when k==0.  
                   -x²/(1-x)² contributes –(n‑1) for n≥2; this combines with +1
                   to give T(n,0)=2‑n for n≥2, and T(0,0)=T(1,0)=1.
                 ---------------------------------------------------------------- */
                if (k == 0) {
                    if (i < 2) {
                        val = 1;        /* T(0,0)=T(1,0)=1 */
                    } else {
                        val = (seq_t)2 - (seq_t)i; /* 2‑i */
                    }
                }

                /* ----------------------------------------------------------------
                   Convolution part for k>0:
                     T(n,k) += Σ_{a=0}^{n-1} Σ_{b=0}^{k-1} T(a,b)·T(n‑1‑a, k‑1‑b)
                   We guard against accessing entries outside the triangular
                   array by checking that 0≤b≤a and 0≤k‑1‑b≤n‑1‑a.
                 ---------------------------------------------------------------- */
                if (k > 0) {
                    for (size_t a = 0; a < i; ++a) {
                        size_t c = i - 1 - a;              /* second row index */
                        size_t maxb = k - 1;               /* maximum b to consider */
                        for (size_t b = 0; b <= maxb; ++b) {
                            if (b > a) continue;           /* T(a,b) not defined */
                            size_t d = k - 1 - b;           /* second column index */
                            if (d > c) continue;           /* T(c,d) not defined */
                            val += rows[a][b] * rows[c][d];
                        }
                    }
                }

                rows[i][k] = val;
            }
        }
    }

    /* --------------------------------------------------------------------
       Extract the value at the requested flattened index n.
     -------------------------------------------------------------------- */
    idx_t offset = r * (r + 1) / 2;    /* first index of row r */
    idx_t col     = n - offset;        /* column inside row r */
    return rows[r][col];
}
