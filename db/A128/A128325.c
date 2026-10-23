/*
 * A128325  Rectangular table, read by antidiagonals, where the g.f.s of row
 *          n, R(x,n), satisfy: R(x,n+1) = R(G(x),n) for n>=0 and x*R(x,0) =
 *          G(x) = x + x*G(G(x)) is the g.f. of A030266.
 * https://oeis.org/A128325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the infinite rectangular table
 *            defined by the generating functions R(x,n) along antidiagonals.
 *            R(x,n) = 1 + G(G(...G(x)...)) (self-composition of G (n+2
 *            times)), where G satisfies G(x) = x + x*G(G(x)). The sequence is
 *            defined for n ≥ 0. The OEIS entry provides the firs...
 * Range:     u8 n <= 25, u16 n <= 52, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility functions for dynamic memory and freeing */
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

/* Forward declarations of helper math functions if we skip the
   library versions.  Use implementation below. */
static idx_t floor_sqrt_u64(idx_t n);
static idx_t compute_s(idx_t n);
static void compose_series(const seq_t *f, const seq_t *g, seq_t *h, size_t deg);

/* --------------------------------------------------------------------- */
/* Integer square root (floor) – safe for 64‑bit values.  */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t sq  = mid * mid;
        if (sq == n) {
            return mid;
        } else if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Compute s satisfying s(s+1)/2 ≤ n < (s+1)(s+2)/2 */
static idx_t compute_s(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 2000;   /* > sqrt(2*10 000) */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t t   = mid * (mid + 1) / 2;
        if (t <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* --------------------------------------------------------------------- */
/* Compose two power series: h(x) = f(g(x)), up to degree deg.
   All series are assumed to be zero at constant term only when relevant.
   For the functions used here deg ≤ 200, so a naïve O(deg²) algorithm
   is sufficient. */
static void compose_series(const seq_t *f,
                           const seq_t *g,
                           seq_t *h,
                           size_t deg)
{
    /* Zero output */
    for (size_t i = 0; i <= deg; ++i) h[i] = 0;

    /* Handle constant term of f */
    h[0] = f[0];

    /* pow   = g⁰ = 1 */
    seq_t *pow = alloc_seq_array(deg + 1);
    pow[0] = 1;
    for (size_t i = 1; i <= deg; ++i) pow[i] = 0;

    for (size_t k = 1; k <= deg; ++k) {
        /* pow = pow * g  (g[0] is usually 0) */
        seq_t *newpow = alloc_seq_array(deg + 1);
        for (size_t i = 0; i <= deg; ++i) {
            if (pow[i] == 0) continue;
            for (size_t j = 1; i + j <= deg; ++j) {  /* g[0] = 0, skip */
                newpow[i + j] = newpow[i + j] + pow[i] * g[j];
            }
        }
        seq_free_array(pow);
        pow = newpow;

        if (f[k] != 0) {
            for (size_t n = 1; n <= deg; ++n) {
                h[n] = h[n] + f[k] * pow[n];
            }
        }
    }

    seq_free_array(pow);
}

/* --------------------------------------------------------------------- */
/* Main sequence function.  This uses static storage to reuse computed
   tables between successive calls. */
seq_t a(idx_t n)
{
    /* Static cached tables */
    static seq_t *G = NULL;           /* G(x) coefficients up to Gdeg */
    static seq_t *Gpow = NULL;        /* Gpow[row][deg] , rows indexed from 0 */
    static size_t Gdeg = 0;           /* current maximum degree stored for G */
    static size_t GpowRows = 0;       /* number of rows stored (max row + 1) */

    /* Determine antidiagonal indices */
    idx_t s = compute_s(n);
    idx_t i = n - s * (s + 1) / 2;   /* degree index */
    idx_t j = s - i;                 /* row index */

    /* The constant coefficient (i==0) is always 1 */
    if (i == 0) return 1;

    /* Required number of rows and degree */
    size_t need_rows = (size_t)(j + 2 + 1);   /* we need row j+2, 0‑based array */
    size_t need_deg  = (size_t)(s);           /* need coefficients up to s */

    /* Recompute tables if necessary */
    if (need_deg > Gdeg || need_rows > GpowRows) {
        /* -------------------------------------------------- */
        /* 1. Compute G(x) up to need_deg */
        /* ----------------------------------------------*/
        seq_t *newG = NULL;
        seq_t *C    = NULL;

        if (G) seq_free_array(G);
        G = alloc_seq_array(need_deg + 1);
        for (size_t k = 0; k <= need_deg; ++k) G[k] = 0;
        if (need_deg >= 1) G[1] = 1;   /* initial guess */

        newG = alloc_seq_array(need_deg + 1);
        C    = alloc_seq_array(need_deg);   /* up to need_deg-1 */

        /* iterative convergence (max 20 iterations; enough for deg≤200) */
        for (size_t it = 0; it < 20; ++it) {
            /* C = G(G(x)) up to degree need_deg‑1 */
            compose_series(G, G, C, need_deg - 1);

            /* newG = 1 + x * C  => newG[0]=0, newG[1]=1, newG[n]=C[n-1] */
            newG[0] = 0;
            if (need_deg >= 1) newG[1] = 1;
            for (size_t k = 2; k <= need_deg; ++k) newG[k] = C[k - 1];

            /* check for convergence */
            bool equal = true;
            for (size_t k = 0; k <= need_deg; ++k) {
                if (newG[k] != G[k]) { equal = false; break; }
            }
            /* copy newG to G (always, to allow further iterations) */
            for (size_t k = 0; k <= need_deg; ++k) G[k] = newG[k];
            if (equal) break;
        }
        seq_free_array(newG);
        seq_free_array(C);
        Gdeg = need_deg;

        /* -------------------------------------------------- */
        /* 2. Build powers Gpow: Gpow[1] = G, Gpow[m] = G(Gpow[m-1])  */
        /* ----------------------------------------------*/
        if (Gpow) seq_free_array(Gpow);
        GpowRows = need_rows;
        Gpow     = alloc_seq_array(GpowRows * (Gdeg + 1));
        size_t row_size = Gdeg + 1;

        /* Row 1 */
        seq_t *row1 = Gpow + 1 * row_size;
        for (size_t k = 0; k <= Gdeg; ++k) row1[k] = G[k];

        /* Allocate temporary buffer for composition */
        seq_t *tmp = alloc_seq_array(Gdeg + 1);

        /* Remaining rows */
        for (size_t m = 2; m < GpowRows; ++m) {
            seq_t *prev = Gpow + (m - 1) * row_size;
            seq_t *curr = Gpow + m * row_size;
            compose_series(G, prev, curr, Gdeg);
        }
        seq_free_array(tmp);
    }

    /* ---------------------------------------------- */
    /* Retrieve the requested coefficient            */
    /* ----------------------------------------------*/
    size_t row_size = Gdeg + 1;
    seq_t *row = Gpow + (j + 2) * row_size;
    return row[i];
}
