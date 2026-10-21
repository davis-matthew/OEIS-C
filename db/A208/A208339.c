/*
 * A208339  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208838; see the Formula section.
 * https://oeis.org/A208339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangle T(n,k) read row‑by‑row where T(0,0)=1, T(1,0)=1,
 *            T(2,0)=1, T(2,1)=3,
 *            T(n,k)=T(n‑1,k)+2*T(n‑1,k‑1)−T(n‑2,k‑1)+T(n‑2,k‑2) for n≥3,
 *            T(n,k)=0 if k<0 or k>n
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for realloc */

/* Helper to grow a dynamic array of seq_t */
static void ensure_capacity(idx_t min_cap, seq_t **p, idx_t *cap)
{
    if (*cap >= min_cap) return;
    *p = (seq_t *)realloc(*p, (size_t)min_cap * sizeof(seq_t));
    *cap = min_cap;
}

/* Compute the n-th element of the flattened triangle */
seq_t a(idx_t n)
{
    /* Find the row r that contains index n (rows are 1‑based). */
    idx_t r = 1;
    while ( (seq_t)r * (r + 1) / 2 < n ) ++r;

    /* Static storage for the flattened triangle. */
    static seq_t *data = NULL;
    static idx_t data_cap = 0;   /* allocated capacity */
    static idx_t rows = 0;       /* last row that has been built */

    /* Total number of entries needed up to row r. */
    idx_t needed = r * (r + 1) / 2;

    /* Grow the storage if needed. */
    ensure_capacity(needed, &data, &data_cap);

    /* Build any missing rows one by one. */
    for (idx_t i = rows + 1; i <= r; ++i) {
        idx_t offset_i = (i - 1) * i / 2;                /* start index of row i (0‑based) */
        if (i == 1) {                                   /* row 1 */
            data[offset_i] = 1;
        }
        else if (i == 2) {                              /* row 2 */
            data[offset_i]     = 1;
            data[offset_i + 1] = 3;
        }
        else {                                          /* i ≥ 3 */
            idx_t offset_prev   = offset_i - (i - 1);          /* start index of row i-1 */
            idx_t offset_prev2  = offset_i - (i - 1) - (i - 2);/* start index of row i-2 */
            for (idx_t j = 0; j < i; ++j) {
                seq_t term = 0;

                /* T(i-1,j) exists only if j ≤ i-2 */
                if (j <= i - 2) {
                    term += data[offset_prev + j];
                }

                /* 2*T(i-1,j-1) exists if j ≥ 1 */
                if (j >= 1) {
                    term += data[offset_prev + (j - 1)] * (seq_t)2;
                }

                /* -T(i-2,j-1) exists if j ≥ 1 and j-1 ≤ i-3 (i.e., j ≤ i-2) */
                if (j >= 1 && j <= i - 2) {
                    term -= data[offset_prev2 + (j - 1)];
                }

                /* +T(i-2,j-2) exists if j ≥ 2 and j-2 ≤ i-3 (automatically true for j ≤ i-1) */
                if (j >= 2) {
                    term += data[offset_prev2 + (j - 2)];
                }

                data[offset_i + j] = term;
            }
        }
    }
    rows = r;

    /* Translate global index n to row r and column col. */
    idx_t offset_r = (r - 1) * r / 2;          /* start index of row r */
    idx_t col = n - offset_r - 1;              /* 0‑based column in row r */
    return data[offset_r + col];
}
