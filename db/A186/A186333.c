/*
 * A186333  Triangle T(n,k), the coefficient of x^n in x^k*(1+x+x^2+2*x^3)^k,
 *          1<=k<=n.
 * https://oeis.org/A186333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A186333
 *  Triangle T(n,k): coefficient of x^n in x^k*(1+x+x^2+2*x^3)^k, 1 <= k <= n.
 *  The sequence is the triangle flattened row by row.
 */

#include <stdlib.h>

/* Representation of a polynomial: coefficients c[0] .. c[len-1] */
typedef struct {
    seq_t *c;      /* coefficient array */
    size_t len;    /* number of coefficients (degree + 1) */
} Poly;

/* Global storage for powers of the base polynomial (1 + x + x^2 + 2x^3) */
static Poly *polys = NULL;      /* polys[0] = 1, polys[1] = base, etc. */
static size_t polys_cnt = 0;    /* number of stored powers (length of polys array) */
static size_t polys_cap = 0;    /* allocated capacity of polys */

/* Ensure that polys contains powers up to (and including) k */
static void ensure_powers(size_t k)
{
    /* Resize global array if necessary */
    if (polys_cap <= k) {
        size_t new_cap = polys_cap == 0 ? 4 : polys_cap * 2;
        if (new_cap <= k) new_cap = k + 1;
        Poly *tmp = (Poly *)realloc(polys, new_cap * sizeof(Poly));
        polys = tmp;
        polys_cap = new_cap;
    }

    /* Initialise first power (k == 0) */
    if (polys_cnt == 0) {
        polys[0].len = 1;
        polys[0].c = (seq_t *)malloc(sizeof(seq_t));
        polys[0].c[0] = (seq_t)1;
        polys_cnt = 1;
    }

    /* Coefficients of the base polynomial: 1 + x + x^2 + 2x^3 */
    static const seq_t base[4] = {(seq_t)1, (seq_t)1, (seq_t)1, (seq_t)2};

    /* Build powers sequentially up to k */
    while (polys_cnt <= k) {
        const Poly *prev = &polys[polys_cnt - 1];
        size_t newlen = prev->len + 3;          /* 3 is the max degree added by the base */
        seq_t *newc = (seq_t *)calloc(newlen, sizeof(seq_t));  /* zero-initialised */

        for (size_t i = 0; i < prev->len; ++i) {
            seq_t ai = prev->c[i];
            if (ai == (seq_t)0) continue;
            newc[i]     += ai;                /* *1 */
            newc[i + 1] += ai;                /* *x */
            newc[i + 2] += ai;                /* *x^2 */
            newc[i + 3] += ai * (seq_t)2;     /* *2x^3 */
        }

        polys[polys_cnt].len = newlen;
        polys[polys_cnt].c   = newc;
        ++polys_cnt;
    }
}

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n > (r-1)r/2 */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Column index k (1-based) */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;            /* 1 <= k <= r */

    /* Ensure we have precomputed the k-th power of the base polynomial */
    ensure_powers((size_t)k);

    /* Exponent of x in the k-th power that gives coefficient for T(r,k) */
    idx_t coef_idx = r - k;             /* a non‑negative integer */

    if (coef_idx < (idx_t)polys[(size_t)k].len) {
        return polys[(size_t)k].c[coef_idx];
    }
    /* Out of range: coefficient is 0 */
    return (seq_t)0;
}
