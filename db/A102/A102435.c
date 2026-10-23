/*
 * A102435  Triangle read by rows: T(n,k) is the number of k-matchings of the
 *          corona L'(n) of the ladder graph L(n)=P_2 X P_n. and the complete
 *          graph K(1); in other words, L'(n) is the graph constructed from
 *          L(n) by adding for each vertex v a new vertex v' and the edge vv'.
 * https://oeis.org/A102435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A102435 – triangle read by rows
   The triangle coefficients T(n,k) satisfy
      P[0] = 1
      P[1] = 1 + 3t + t²
      P[2] = 1 + 8t + 16t² + 8t³ + t⁴
      P[n] = (1 + 4t + t²)P[n‑1] + t(1+t)²P[n‑2] – t³P[n‑3]   (n≥3)
   where P[n] = Σ T(n,k) tᵏ, 0≤k≤2n.
   The sequence a(n) lists the triangle entries row by row.
   For an index n ≥ 0:
        row   = ⌊√n⌋
        col   = n – row²
        a(n)  = T(row, col).
   The implementation caches whole rows so that calls in any order are
   answered in O(1) after O(max_row) preprocessing.  All arithmetic
   is performed in the type seq_t provided by the build.  The type idx_t
   is an unsigned 64‑bit index. */

#include <stdlib.h>
#include <math.h>

/* static storage for the computed rows */
static seq_t **rows = NULL;       /* rows[i] points to array of 2*i+1 entries */
static idx_t rows_level = 0;      /* number of rows stored (rows[0] … rows[rows_level-1]) */

/* ensure that rows 0 … m are allocated and filled */
static void grow_rows_up_to(idx_t m)
{
    if (rows_level > m)
        return;                                           /* already present */

    /* extend pointer array */
    if (rows_level == 0) {
        rows = (seq_t **)malloc((m + 1) * sizeof(seq_t *));
    } else {
        rows = (seq_t **)realloc(rows, (m + 1) * sizeof(seq_t *));
    }
    if (!rows)
        abort();                                           /* allocation failure */
    for (idx_t i = rows_level; i <= m; ++i)
        rows[i] = NULL;
    rows_level = m + 1;

    /* build rows sequentially */
    for (idx_t n = 0; n <= m; ++n) {
        if (rows[n] != NULL)
            continue;                                       /* already computed earlier */

        /* base cases */
        if (n == 0) {
            rows[0] = (seq_t *)malloc(sizeof(seq_t));
            rows[0][0] = (seq_t)1;
            continue;
        }
        if (n == 1) {
            rows[1] = (seq_t *)malloc(3 * sizeof(seq_t));
            rows[1][0] = (seq_t)1;
            rows[1][1] = (seq_t)3;
            rows[1][2] = (seq_t)1;
            continue;
        }
        if (n == 2) {
            rows[2] = (seq_t *)malloc(5 * sizeof(seq_t));
            rows[2][0] = (seq_t)1;
            rows[2][1] = (seq_t)8;
            rows[2][2] = (seq_t)16;
            rows[2][3] = (seq_t)8;
            rows[2][4] = (seq_t)1;
            continue;
        }

        /* recurrence for n ≥ 3 */
        size_t sz = 2 * n + 1;
        rows[n] = (seq_t *)malloc(sz * sizeof(seq_t));
        seq_t *row_nm1 = rows[n - 1];
        seq_t *row_nm2 = rows[n - 2];
        seq_t *row_nm3 = rows[n - 3];
        size_t sz_nm1 = 2 * (n - 1) + 1;
        size_t sz_nm2 = 2 * (n - 2) + 1;
        size_t sz_nm3 = 2 * (n - 3) + 1;

        for (size_t k = 0; k < sz; ++k) {
            seq_t sum = 0;

            /* contributions from P[n-1] : (1 + 4t + t²) */
            if (k < sz_nm1)              sum += row_nm1[k];
            if (k >= 1 && k - 1 < sz_nm1)sum += (seq_t)4 * row_nm1[k - 1];
            if (k >= 2 && k - 2 < sz_nm1)sum += row_nm1[k - 2];

            /* contributions from P[n-2] : t(1+t)² = t + 2t² + t³ */
            if (k >= 1 && k - 1 < sz_nm2)sum += row_nm2[k - 1];
            if (k >= 2 && k - 2 < sz_nm2)sum += (seq_t)2 * row_nm2[k - 2];
            if (k >= 3 && k - 3 < sz_nm2)sum += row_nm2[k - 3];

            /* subtract t³ * P[n-3] */
            if (k >= 3 && k - 3 < sz_nm3)sum -= row_nm3[k - 3];

            rows[n][k] = sum;
        }
    }
}

/* public interface */
seq_t a(idx_t n)
{
    /* determine row and column from the global index */
    idx_t row = (idx_t)floor(sqrt((long double)n));
    /* adjust in case of rounding errors */
    while ((row + 1) * (row + 1) <= n)
        ++row;
    while (row * row > n)
        --row;
    idx_t col = n - row * row;          /* 0 ≤ col ≤ 2·row */

    grow_rows_up_to(row);
    return rows[row][col];
}
