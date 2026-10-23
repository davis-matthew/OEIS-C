/*
 * A151511  The triangle in A151359 read by rows downwards.
 * https://oeis.org/A151511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   A151511 is the triangle T(n,k) of partitions of n labelled
 *            elements into k blocks each of size ≤6. Recurrence: T(0,0)=1,
 *            T(n,0)=0 (n>0). For n>0, k>0: T(n,k) = Σ_{i=1..min(6, n-(k-1))}
 *            C(n-1, i-1) * T(n-i, k-1)
 * Range:     u8 n <= 30, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- required headers ---------- */
#include <stdint.h>
#include <stdlib.h>

/* ---------- helper: binomial coefficient C(n,k) for k≤5 (exact) ---------- */
static inline seq_t binom_small(idx_t n, int k)
{
    if (k <= 0) return 1;
    if (k > (int)n) return 0;
    if (k > (int)n - k) k = (int)n - k;   /* symmetry */
    seq_t res = 1;
    for (int i = 1; i <= k; ++i) {
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

/* ---------- private storage for the triangle ---------- */
static seq_t **tri = NULL;    /* array of pointers to rows */
static idx_t rows_count = 0;  /* number of rows currently stored */

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    /* answer is T(r, c) where r = row number, c = column within row */
    /* find row r (0‑based) containing linear index n */
    idx_t r = ((idx_t)((__int128)8 * n + 1) >> 1);  /* 8n+1; using 128-bit to avoid overflow on very large n */
    while ((r + 1) * r / 2 <= n) r++;
    r--;   /* now r*(r+1)/2 > n, so r is last row that might contain n */
    r = ((idx_t)((__int128)8 * n + 1) >> 1);   /* recompute with integer sqrt? use loop below */
    /* compute proper r by decreasing until r*(r+1)/2 <= n */
    while (r * (r + 1) / 2 > n) r--;
    /* ensure rows up to r are computed */
    while (rows_count <= r) {
        idx_t cur = rows_count;
        /* allocate row of length cur+1, initialize to zero */
        seq_t *row = (seq_t*)calloc((size_t)(cur + 1), sizeof(seq_t));
        if (!row) abort();  /* OOM */
        if (cur == 0) {
            row[0] = 1;      /* T(0,0)=1 */
        } else {
            row[0] = 0;      /* T(cur,0)=0 */
            for (idx_t k = 1; k <= cur; ++k) {
                seq_t sum = 0;
                /* minimal remaining elements after choosing i for the block
                   containing the smallest element must be ≥ k‑1  */
                idx_t max_i = 6;
                if (max_i > cur - (k - 1)) max_i = cur - (k - 1);
                for (idx_t i = 1; i <= max_i; ++i) {
                    seq_t coef = binom_small(cur - 1, (int)i - 1);
                    sum += coef * tri[cur - i][k - 1];
                }
                row[k] = sum;
            }
        }
        /* append newly built row to the table */
        seq_t **tmp = (seq_t**)realloc(tri, (size_t)(rows_count + 1) * sizeof(seq_t*));
        if (!tmp) abort();   /* OOM */
        tri = tmp;
        tri[rows_count] = row;
        rows_count++;
    }

    /* column index within row r */
    idx_t row_start = r * (r + 1) / 2;   /* offset of row r */
    idx_t c = n - row_start;             /* column in row r */

    return tri[r][c];
}
