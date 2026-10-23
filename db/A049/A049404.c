/*
 * A049404  Triangle read by rows, the Bell transform of n!*binomial(2,n)
 *          (without column 0).
 * https://oeis.org/A049404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   row_of_position
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A049404 – triangle read by rows
    Recurrence:  a(n,m) = (3*m - n + 1)*a(n-1,m) + a(n-1,m-1)
    with a(1,1)=1, a(n,0)=0, a(n,m)=0 when m>n.
    a(idx_t n) returns the nth term of the triangle read row‑by‑row.
*/

#include <stdlib.h>   /* malloc, realloc, abort */
#include "../utils/row_of_position.h"

/* static storage for the triangle values (1‑based rows, 1‑based columns) */
static seq_t *tri = NULL;
static idx_t tri_size = 0;        /* allocated number of elements */
static idx_t max_row_precomp = 0; /* highest row computed so far */

/* ensure we have space for at least 'needed' elements */
static void ensure_tri_capacity(idx_t needed) {
    if (tri_size >= needed) return;
    idx_t new_size = tri_size ? tri_size * 2 : 16;
    if (new_size < needed) new_size = needed;
    seq_t *tmp = (seq_t*)realloc(tri, new_size * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    tri = tmp;
    tri_size = new_size;
}

/* compute all rows up to 'upto' (inclusive) using the recurrence */
static void precompute_rows(idx_t upto) {
    while (max_row_precomp < upto) {
        ++max_row_precomp;
        idx_t n = max_row_precomp;
        idx_t base = n * (n - 1) / 2;                /* first index of row n */
        ensure_tri_capacity(base + n);               /* need n new elements */

        if (n == 1) {                               /* base case a(1,1)=1 */
            tri[0] = 1;
            continue;
        }

        idx_t base_prev = (n - 1) * (n - 2) / 2;      /* start of previous row */

        for (idx_t m = 1; m <= n; ++m) {
            seq_t prev_val = 0;
            if (m <= n - 1)
                prev_val = tri[base_prev + m - 1];

            seq_t sub_val = 0;
            if (m > 1)                              /* a(n-1, m-1) */
                sub_val = tri[base_prev + (m - 1) - 1];

            int coeff = 3 * m - n + 1;               /* may be non‑positive */
            seq_t term1 = 0;
            if (coeff > 0)
                term1 = (seq_t)coeff * prev_val;    /* multiplication safe for positive coeff */

            tri[base + m - 1] = term1 + sub_val;
        }
    }
}

/* public function – returns the nth term of the triangle read by rows */
seq_t a(idx_t n) {
    if (n < 1) return 0;  /* behaviour below offset is irrelevant */
    idx_t r = row_of_position(n);
    idx_t offset_before = r * (r - 1) / 2;   /* elements before row r */
    idx_t col = n - offset_before;          /* 1‑based column within row r */

    precompute_rows(r);
    return tri[offset_before + col - 1];
}
