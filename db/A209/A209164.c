/*
 * A209164  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209165; see the Formula section.
 * https://oeis.org/A209164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   u(1,x)=1, v(1,x)=1 u(n,x)=u(n-1,x)+(x+1)*v(n-1,x)
 *            v(n,x)=(x+1)*u(n-1,x)+v(n-1,x)+1 a(n) = coefficient of x^(k-1)
 *            in u(r,x) where r = smallest integer with r*(r+1)/2 ≥ n k = n -
 *            r*(r-1)/2
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *u = NULL;
static seq_t *v = NULL;
static seq_t *nu = NULL;
static seq_t *nv = NULL;
static idx_t cur_n = 0;        /* current row that u and v represent */
static size_t alloc_cap = 0;   /* current allocated capacity */

/* Find the row number r for index n */
static idx_t find_row(idx_t n) {
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t t = (mid * (mid + 1)) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Expand the dynamic arrays so that u and v hold the coefficients
   for row 'target' (row numbers start at 1). */
static void ensure_capacity(idx_t target) {
    /* Lazy initialization */
    if (cur_n == 0) {
        alloc_cap = 1;
        u  = (seq_t *)malloc(sizeof(seq_t) * alloc_cap);
        v  = (seq_t *)malloc(sizeof(seq_t) * alloc_cap);
        nu = (seq_t *)malloc(sizeof(seq_t) * alloc_cap);
        nv = (seq_t *)malloc(sizeof(seq_t) * alloc_cap);
        u[0] = 1;
        v[0] = 1;
        cur_n = 1;
    }

    /* Grow row by row up to the required target */
    while (cur_n < target) {
        idx_t n = cur_n + 1;                /* new row number */
        if ((size_t)n > alloc_cap) {        /* enlarge allocated arrays if needed */
            alloc_cap = n;
            u  = (seq_t *)realloc(u,  sizeof(seq_t) * alloc_cap);
            v  = (seq_t *)realloc(v,  sizeof(seq_t) * alloc_cap);
            nu = (seq_t *)realloc(nu, sizeof(seq_t) * alloc_cap);
            nv = (seq_t *)realloc(nv, sizeof(seq_t) * alloc_cap);
        }

        /* Compute coefficients for row n into nu and nv */
        for (idx_t d = 0; d < n; ++d) {
            seq_t a = (d <= n - 2) ? u[d] : 0;
            seq_t b = (d <= n - 2) ? v[d] : 0;
            seq_t c = (d > 0)     ? v[d - 1] : 0;
            nu[d] = a + b + c;
        }
        for (idx_t d = 0; d < n; ++d) {
            seq_t d1 = (d <= n - 2) ? u[d] : 0;
            seq_t d2 = (d > 0)     ? u[d - 1] : 0;
            seq_t d3 = (d <= n - 2) ? v[d] : 0;
            if (d == 0)
                nv[d] = d1 + d3 + 1;
            else
                nv[d] = d1 + d2 + d3;
        }

        /* Swap pointers so that u/v now point to the new row */
        seq_t *tmp = u;  u = nu;  nu = tmp;
        tmp = v;         v = nv;  nv = tmp;
        cur_n = n;
    }
}

seq_t a(idx_t n) {
    if (n < 1)
        return 0;                 /* undefined for n < 1, but return 0 */

    idx_t r = find_row(n);                                 /* row */
    idx_t pos = n - (r - 1) * r / 2;                       /* position in row (1‑based) */

    ensure_capacity(r);                                    /* ensure that row r is computed */

    return u[pos - 1];   /* coefficient of degree pos-1 of u(r,x) */
}
