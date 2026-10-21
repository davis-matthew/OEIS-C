/*
 * A208760  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208759; see the Formula section.
 * https://oeis.org/A208760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n)th element of the triangle T defined by T(0,0)=1,
 *            T(1,0)=1, T(2,0)=1, T(1,1)=0, T(2,1)=3, T(2,2)=0, and for n>=3,
 *            0<=k<=n: T(n,k) = T(n-1,k) + 2*T(n-1,k-1) + 2*T(n-2,k-2). a(n)
 *            is obtained by flattening the triangle row‑by‑row starting with
 *            n=1
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *tbl = NULL;      /* triangular storage flat array */
static size_t rows = 0;        /* number of rows already computed (0 means none) */
static size_t tbl_size = 0;    /* current allocation size */

static void ensure_rows(idx_t r)
{
    /* compute rows up to r inclusive */
    if (rows > r) return;

    size_t new_rows = r + 1;                  /* target number of rows */
    size_t new_size = new_rows * (new_rows + 1) / 2; /* total entries needed */

    tbl = (seq_t *)realloc(tbl, new_size * sizeof(seq_t));
    /* recalculate entries for each newly added row */
    for (size_t n = rows; n < new_rows; ++n) {
        size_t base_n = n * (n + 1) / 2;     /* starting index of row n */
        if (n == 0) {
            tbl[base_n] = 1;
            continue;
        }
        if (n == 1) {
            tbl[base_n + 0] = 1;
            tbl[base_n + 1] = 0;
            continue;
        }
        if (n == 2) {
            tbl[base_n + 0] = 1;
            tbl[base_n + 1] = 3;
            tbl[base_n + 2] = 0;
            continue;
        }
        /* general recurrence for n >= 3 */
        for (size_t k = 0; k <= n; ++k) {
            seq_t val = 0;
            /* T(n-1,k) */
            if (k <= n - 1) {
                val += tbl[(n - 1) * n / 2 + k];
            }
            /* 2 * T(n-1,k-1) */
            if (k >= 1 && k - 1 <= n - 1) {
                val += (tbl[(n - 1) * n / 2 + (k - 1)] * 2);
            }
            /* 2 * T(n-2,k-2) */
            if (k >= 2 && k - 2 <= n - 2) {
                val += (tbl[(n - 2) * (n - 1) / 2 + (k - 2)] * 2);
            }
            tbl[base_n + k] = val;
        }
    }
    rows = new_rows;
    tbl_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; behaviour below 1 is irrelevant */

    /* find the row r that contains position n */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) ++r;

    ensure_rows(r);

    idx_t base_prev = (r - 1) * r / 2;            /* first index of previous row */
    size_t offset = (size_t)(n - base_prev - 1);  /* position within the row (0‑based) */
    size_t idx = (size_t)(r * (r + 1) / 2 + offset);

    return tbl[idx];
}
