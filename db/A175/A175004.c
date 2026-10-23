/*
 * A175004  Interspersion related to the Wythoff Array.
 * https://oeis.org/A175004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of A175004, read in anti‑diagonal
 *            order. The array is defined row‑wise by: a(r,1) =
 *            (r==1?1:floor(r*φ²)-2) a(r,2) = a(r,1) + floor(r*φ) a(r,j) =
 *            a(r,j-1) + a(r,j-2) + 1 for j≥3. Correspondence of linear index
 *            to 2‑D coordinates: Let d be the smallest integer with
 *            d(d+1)/2...
 * Range:     u8 n <= 56, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

static const double PHI  = (1.0 + sqrt(5.0)) / 2.0;
static const double PHI2 = PHI * PHI;   /* φ² */

static inline idx_t floor_phi(idx_t n)
{
    return (idx_t)((double)n * PHI + 1e-12);
}

static inline idx_t floor_phi2(idx_t n)
{
    return (idx_t)((double)n * PHI2 + 1e-12);
}

/* static cache */
static seq_t  **table = NULL;
static idx_t   cached_rows = 0;

/* rebuild table up to needed rows */
static void rebuild_table(idx_t needed)
{
    /* free old */
    if (table) {
        for (idx_t r = 1; r <= cached_rows; ++r)
            free(table[r]);
        free(table);
    }

    cached_rows = needed;
    table = (seq_t**)malloc((cached_rows + 1) * sizeof(seq_t*));

    for (idx_t r = 1; r <= cached_rows; ++r) {
        table[r] = (seq_t*)malloc((cached_rows + 1) * sizeof(seq_t));

        idx_t a1_i = (r == 1) ? 1 : floor_phi2(r) - 2;
        idx_t a2_i = a1_i + floor_phi(r);

        table[r][1] = (seq_t)a1_i;
        if (cached_rows >= 2) table[r][2] = (seq_t)a2_i;

        for (idx_t j = 3; j <= cached_rows; ++j)
            table[r][j] = table[r][j-1] + table[r][j-2] + 1;
    }
}

seq_t a(idx_t n)
{
    /* handle offset starting at 1 */
    if (n == 0) return (seq_t)0;

    /* find diagonal d: minimal d with d(d+1)/2 >= n */
    idx_t d = 0;
    while ((d * (d + 1)) / 2 < n)
        ++d;

    /* ensure table covers rows up to d */
    if (cached_rows < d)
        rebuild_table(d);

    /* compute indices inside diagonal */
    idx_t t_prev  = (d - 1) * d / 2;
    idx_t offset  = n - t_prev - 1;           /* 0‑based in diagonal */
    idx_t row = offset + 1;
    idx_t col = d - offset;

    return table[row][col];
}
