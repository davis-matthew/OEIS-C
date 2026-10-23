/*
 * A108990  Triangle, read by rows, where the g.f. of row n, R_n(x), is a
 *          polynomial of degree n that satisfies: [x^k] R_{n+1}(x) = [x^k] (1
 *          + x*R_n(x))^(n+1) for k=0..n+1, with R_0(x) = 1.
 * https://oeis.org/A108990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   addmul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A108990 – Triangle read by rows.
    R₀(x) = 1
    R_{n+1}(x) = (1 + x·R_n(x))^{n+1}  (coefficients are taken for 0 ≤ k ≤ n+1)
    The sequence a(n) is the triangle read row‑wise.
    a(0)=1 (row 0), a(1)=1,a(2)=1 (row 1), etc.                         */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/addmul.h"

/* ------------------------------------------------------------------ */
/* Static state:  pointers to completed rows and their capacities.     */
/* ------------------------------------------------------------------ */
static seq_t **rows = NULL;    /* rows[r] points to array of r+1 elements */
static idx_t rows_cap = 0;     /* allocated capacity of rows[]          */
static idx_t rows_size = 0;    /* number of rows actually computed     */

/* ------------------------------------------------------------------ */
/* Return the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.        */
/* The computation uses a 128‑bit intermediate to avoid overflow.      */
/* ------------------------------------------------------------------ */
static inline idx_t row_of_index(idx_t n)
{
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

/* ------------------------------------------------------------------ */
/* Add the product a*b to *acc with wrap‑around modulo 2^width(seq_t). */
/* For 8‑byte or smaller seq_t, 128‑bit arithmetic is used to avoid
   overflow before truncation. For larger seq_t (arbitrary‑precision),
   normal arithmetic is sufficient.                                     */

/* ------------------------------------------------------------------ */
/* Ensure that rows[0..r] are computed and stored.                     */
/* ------------------------------------------------------------------ */
static void ensure_rows(idx_t r)
{
    if (r < rows_size)
        return;                 /* already available */

    /* enlarge rows array if needed */
    if (r + 1 > rows_cap) {
        size_t new_cap = rows_cap ? rows_cap << 1 : 4;
        while (new_cap <= (size_t)r + 1)
            new_cap <<= 1;
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp) abort();      /* allocation failed */
        rows     = tmp;
        rows_cap = new_cap;
    }

    /* compute new rows from rows_size up to r */
    for (idx_t cur = rows_size; cur <= r; ++cur) {
        size_t len = (size_t)(cur + 1);          /* number of coefficients in this row */
        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
        if (!row) abort();
        rows[cur] = row;

        if (cur == 0) {                       /* R₀(x) = 1 */
            row[0] = (seq_t)1;
            continue;
        }

        /* base polynomial: B(x) = 1 + x·R_{cur‑1}(x) */
        seq_t *base = (seq_t*)malloc(len * sizeof(seq_t));
        if (!base) abort();
        base[0] = (seq_t)1;
        seq_t *prev = rows[cur - 1];
        for (idx_t j = 1; j <= cur; ++j)
            base[j] = prev[j - 1];            /* shift by one place */

        /* poly = B(x)⁰   (initialised to 1) */
        seq_t *poly = (seq_t*)calloc(len, sizeof(seq_t));
        if (!poly) abort();
        poly[0] = (seq_t)1;

        /* tmp will hold the intermediate product of poly * base */
        seq_t *tmp = (seq_t*)calloc(len, sizeof(seq_t));
        if (!tmp) abort();

        /* multiply poly by base cur times to obtain B(x)^cur */
        for (idx_t m = 0; m < cur; ++m) {
            /* clear tmp */
            for (idx_t i = 0; i <= cur; ++i)
                tmp[i] = 0;

            /* poly * base → tmp (degree ≤ cur) */
            for (idx_t i = 0; i <= cur; ++i) {
                seq_t ai = poly[i];
                if (!ai) continue;
                for (idx_t j = 0; i + j <= cur; ++j)
                    addmul(&tmp[i + j], ai, base[j]);
            }
            /* move tmp into poly for next iteration */
            for (idx_t i = 0; i <= cur; ++i)
                poly[i] = tmp[i];
        }

        /* copy the resulting coefficients into the row */
        for (idx_t i = 0; i <= cur; ++i)
            row[i] = poly[i];

        free(base);
        free(poly);
        free(tmp);
    }

    rows_size = r + 1;                    /* rows[0..r] now ready */
}

/* ------------------------------------------------------------------ */
/* Public function requested by the framework.                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);
    idx_t k = n - r * (r + 1) / 2;        /* position within the row */
    ensure_rows(r);
    return rows[r][k];
}
