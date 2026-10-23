/*
 * A125605  Triangle read by rows: T(n,k) = (T(n-1,k-1) + T(n-1,k)) /
 *          gcd(T(n-1,k-1), T(n-1,k)).
 * https://oeis.org/A125605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 156, u16 n <= 415, u32 n <= 2039, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   alloc_seq_array, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Triangle read by rows:
 *   T(0,0)   = 1
 *   T(n,0)   = T(n,n) = 1   for n > 0
 *   T(n,k)   = (T(n-1,k-1) + T(n-1,k)) / gcd(T(n-1,k-1), T(n-1,k))
 *
 * a(n) is the n‑th element read row‑by‑row, with a(0)=1.
 * The sequence is generated on demand and memoised across calls.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/alloc_seq_array.h"

/* ---- Helper: triangular number (row start index) ---- */
static idx_t triangular(idx_t r)
{
    return r * (r + 1) / 2;
}

/* ---- Storage for the triangle rows ---- */
static seq_t **triangle     = NULL;   /* array of pointers to rows */
static size_t   tri_cap     = 0;      /* capacity of the triangle array */
static size_t   rows_built  = 0;      /* number of rows that have been built */

/* ---- Ensure the triangle pointers array is large enough ---- */
static void ensure_tri_cap(size_t need)
{
    if (tri_cap <= need) {
        size_t new_cap = tri_cap ? tri_cap * 2 : 1;
        while (new_cap <= need) new_cap *= 2;
        seq_t **tmp = (seq_t **)realloc(triangle, new_cap * sizeof(seq_t *));
        if (!tmp) abort();            /* out of memory */
        triangle = tmp;
        tri_cap  = new_cap;
    }
}

/* ---- Build a single row of the triangle ---- */
static void build_row(idx_t r)
{
    /* r is the index of the new row to build (r >= 0) */
    size_t len = (size_t)(r + 1);
    seq_t *row = alloc_seq_array(len);
    if (!row) abort();

    row[0] = 1;
    row[r] = 1;

    if (r >= 2) {
        for (idx_t k = 1; k < r; ++k) {
            seq_t left  = triangle[r - 1][k - 1];
            seq_t right = triangle[r - 1][k];
            seq_t g = gcd(left, right);
            seq_t a = left / g;
            seq_t b = right / g;
            /* a + b cannot overflow seq_t; use unsigned __int128 if needed */
            seq_t sum = a + b;
            row[k] = sum;
        }
    }

    ensure_tri_cap(r);
    triangle[r] = row;
    ++rows_built;   /* next row to build will be rows_built */
}

/* ---- Public function -- */
seq_t a(idx_t n)
{
    /* Find the row r that contains index n */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;

    /* Ensure the triangle is built up to row r */
    if (rows_built == 0) {
        /* Build row 0 first */
        build_row(0);         /* rows_built == 1 now */
    }
    while (rows_built <= (size_t)r) {
        build_row(rows_built);
    }

    idx_t k = n - triangular(r);   /* column within the row */
    return triangle[r][k];
}
