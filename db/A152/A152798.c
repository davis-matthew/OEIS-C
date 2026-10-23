/*
 * A152798  Triangle defined by T(n,k) = Sum_{j=0..k} C(k,j)*T(n-1,j+k) for
 *          n>k>0 with T(n,0)=T(n,n)=1, read by rows.
 * https://oeis.org/A152798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   _binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152798:
   Triangle T(n,k) = Σ_{j=0..k} C(k,j) * T(n-1,j+k)  (n>k>0), with T(n,0)=T(n,n)=1.
   a(n) is read by rows of the triangle, row 0 first. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_binom_n_k.h"

/* ---------- helpers ---------- */

/* Integer square root of a 64‑bit unsigned integer. */
static inline idx_t _isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 62;               /* highest power of four  ≤ x */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ---------- triangle storage ---------- */

static seq_t **_rows = NULL;     /* array of row pointers           */
static idx_t _rows_cap   = 0;     /* allocated capacity (in indices) */
static idx_t _rows_count = 0;     /* number of rows already computed */

/* Ensure the rows array can hold indices up to 'needed'. */
static void _grow_rows(idx_t needed)
{
    if (needed < _rows_cap) return;
    idx_t new_cap = needed + 1;
    seq_t **tmp = (seq_t **)realloc(_rows, new_cap * sizeof(seq_t *));
    if (!tmp) abort();                 /* allocation failure */
    for (idx_t i = _rows_cap; i < new_cap; ++i) tmp[i] = NULL;
    _rows = tmp;
    _rows_cap = new_cap;
}

/* Compute all rows up to and including 'target' (row number). */
static void _compute_rows_upto(idx_t target)
{
    if (target < _rows_count) return;          /* already done */

    _grow_rows(target);

    for (idx_t r = _rows_count; r <= target; ++r) {
        seq_t *row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        if (!row) abort();                    /* allocation failure */

        row[0] = 1;                           /* left boundary     */
        row[r] = 1;                           /* right boundary    */

        if (r >= 1) {
            for (idx_t k = 1; k <= r - 1; ++k) {
                seq_t sum = 0;
                for (idx_t j = 0; j <= k; ++j) {
                    idx_t m = j + k;
                    seq_t prev = 0;
                    if (m < r) prev = _rows[r - 1][m];
                    seq_t bin = _binom_n_k(k, j);
                    sum += bin * prev;          /* bin*prev fits seq_t if final sum fits */
                }
                row[k] = sum;
            }
        }
        _rows[r] = row;
        _rows_count = r + 1;                  /* next row to compute */
    }
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    /* Find the row number 'r' that contains index n:
       r = floor((sqrt(8*n+1)-1)/2). */
    idx_t t = (idx_t)(((__int128)8 * n) + 1);
    idx_t sqrt_t = _isqrt_u64(t);
    idx_t r = (sqrt_t - 1) / 2;

    _compute_rows_upto(r);                    /* ensure required row exists */

    /* Compute offset of n within row r. */
    idx_t start = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t k = n - start;                      /* 0 ≤ k ≤ r */

    return _rows[r][k];
}
