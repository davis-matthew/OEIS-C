/*
 * A209146  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209147; see the Formula section.
 * https://oeis.org/A209146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * u(1,x)=1, v(1,x)=1 * u(n,x) = u(n-1,x) + (x+1)*v(n-1,x) *
 *            v(n,x) = u(n-1,x) + 2x*v(n-1,x) + 1 * a(n) is the coefficient of
 *            x^{c-1} in u(r,x) where * r(r+1)/2 ≥ n > (r-1)r/2, i.e. the nth
 *            entry in the row‑major * reading of the triangle of coefficients
 *            of u(n,x)
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for precomputed rows of u and v */
static seq_t **uRows = NULL;
static seq_t **vRows = NULL;
static size_t rowsComputed = 0;
static size_t rowsAllocated = 0;

/* ensure capacity for at least 'need' rows of pointers */
static void ensure_rows_capacity(size_t need)
{
    if (rowsAllocated < need) {
        size_t newCap = rowsAllocated ? rowsAllocated : 4;
        while (newCap < need) newCap <<= 1;
        uRows = (seq_t**)realloc(uRows, newCap * sizeof(seq_t*));
        vRows = (seq_t**)realloc(vRows, newCap * sizeof(seq_t*));
        rowsAllocated = newCap;
    }
}

/* build rows up to and including target (1‑based) */
static void build_to_row(size_t target)
{
    for (size_t r = rowsComputed + 1; r <= target; ++r) {
        size_t len = r;
        seq_t *newU = (seq_t*)malloc(len * sizeof(seq_t));
        seq_t *newV = (seq_t*)malloc(len * sizeof(seq_t));

        if (r == 1) {
            newU[0] = (seq_t)1;
            newV[0] = (seq_t)1;
        } else {
            seq_t *prevU = uRows[r - 2];
            seq_t *prevV = vRows[r - 2];
            size_t prevLen = r - 1;

            /* first coefficient */
            newU[0] = prevU[0] + prevV[0];
            newV[0] = prevU[0] + (seq_t)1;

            /* remaining coefficients */
            for (size_t k = 1; k < len; ++k) {
                seq_t u_k = (k < prevLen) ? prevU[k] : 0;
                seq_t v_k = (k < prevLen) ? prevV[k] : 0;
                seq_t v_k_1 = prevV[k - 1];   /* always valid : k-1 < prevLen */

                newV[k] = u_k + (seq_t)2 * v_k_1;
                newU[k] = u_k + v_k + v_k_1;
            }
        }

        ensure_rows_capacity(r);
        uRows[r - 1] = newU;
        vRows[r - 1] = newV;
        ++rowsComputed;
    }
}

/* compute 1‑based row and column from flattened index n */
static void row_col_from_index(idx_t n, idx_t *row, idx_t *col)
{
    /* binary search for smallest r with r(r+1)/2 ≥ n */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
        if (high == 0) {          /* overflow guard – cap at max value */
            high = ~((idx_t)0);
            break;
        }
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    *row = low;
    idx_t prevTri = (unsigned __int128)(low - 1) * low / 2;
    *col = n - prevTri;          /* 1‑based column */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety guard – undefined input */

    idx_t r, c;
    row_col_from_index(n, &r, &c);
    build_to_row((size_t)r);
    return uRows[r - 1][c - 1];
}
