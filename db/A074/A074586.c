/*
 * A074586  Triangle of Moebius polynomial coefficients, read by rows, the
 *          n-th row forming the polynomial M(n,x) such that M(n,-1) = mu(n),
 *          the Moebius function of n.
 * https://oeis.org/A074586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 161, u32 n <= 609, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A074586 – Triangle of Möbius polynomial coefficients (flattened by rows).
 *
 * Define T(n,1)=1. For n≥k>1:
 *   T(n,k)=∑_{m=k-1}^{n-1} ⌊n/m⌋ · T(m,k-1) .
 * Let r be the unique integer with r(r−1)/2 < n ≤ r(r+1)/2;
 * the column index in that row is k = n−r(r−1)/2.
 * Then a(n)=T(r,k).
 */

#include <stdlib.h>   /* malloc, calloc, realloc, abort */
#include <stddef.h>   /* size_t */

/*  Cached rows: rows[i] points to an array of length i+1 (index 0 unused).
 *  rows[0] is never used.  */
static seq_t **rows = NULL;
static idx_t  computed_rows = 0;   /* largest row computed so far */
static size_t rows_cap    = 0;     /* capacity of the rows array */

/* Ensure rows array can hold at least (need+1) pointers. */
static void ensure_rows_cap(idx_t need)
{
    if ((size_t)(need + 1) > rows_cap) {
        size_t new_cap = (size_t)(need + 1) * 2;
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp) abort();               /* allocation failed */
        rows = tmp;
        rows_cap = new_cap;
    }
}

/* Compute and cache rows up to and including 'need'. */
static void ensure_rows_up_to(idx_t need)
{
    if (need <= computed_rows) return;   /* nothing to do */

    ensure_rows_cap(need);

    for (idx_t n = computed_rows + 1; n <= need; ++n) {
        /* Allocate array of length n+1 (index 0 unused). */
        seq_t *row = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!row) abort();               /* allocation failed */

        row[1] = (seq_t)1;                /* T(n,1)=1 */

        /* Compute T(n,k) for k = 2 … n.  */
        for (idx_t k = 2; k <= n; ++k) {
            seq_t sum = (seq_t)0;
            for (idx_t m = k - 1; m <= n - 1; ++m) {
                idx_t f = n / m;         /* ⌊n/m⌋ */
                sum += (seq_t)f * rows[m][k - 1];
            }
            row[k] = sum;
        }
        rows[n] = row;
    }
    computed_rows = need;
}

/* Return the n-th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safety for n<1 */

    /* Find r such that r(r-1)/2 < n ≤ r(r+1)/2. */
    idx_t r = 1;
    while (((r * (r + 1)) / 2) < n) ++r;

    /* Ensure the required rows are complete. */
    ensure_rows_up_to(r);

    /* Column index within the row. */
    idx_t prev_tri = ((r - 1) * r) / 2;
    idx_t k = n - prev_tri;              /* 1‑based column index */

    return rows[r][k];
}
