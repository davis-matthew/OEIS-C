/*
 * A050143  A(n,k) = Sum_{h=0..n-1, m=0..k} A(h,m) for n >= 1 and k >= 1, with
 *          A(n,0) = 1 for n >= 0 and A(0,k) = 0 for k >= 1; square array A,
 *          read by descending antidiagonals.
 * https://oeis.org/A050143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) entry read by descending antidiagonals from
 *            the square array A defined by A(n,0) = 1 (n ≥ 0) , A(0,k) = 0 (k
 *            ≥ 1) , A(0,0) = 1 , A(n,k) = 2*A(n-1,k) + A(n,k-1) – A(n-1,k-1)
 *            (n ≥ 1, k ≥ 2) with initial values A(n,1) = 2^n – 1 (obtained by
 *            the recurrence for k=1). The sequence a(...
 * Range:     u8 n <= 50, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  ----  static cache  ----  */
static seq_t *A_cache = NULL;     /* flattened 2‑D array */
static idx_t A_rows  = 0;         /* cached rows  = max_i + 1  */
static idx_t A_cols  = 0;         /* cached cols  = max_j + 1  */

/* re‑allocate the cache and recompute all values up to new dimensions */
static void ensure_table(idx_t need_i, idx_t need_j)
{
    idx_t new_rows = need_i + 1;
    idx_t new_cols = need_j + 1;

    if (new_rows <= A_rows && new_cols <= A_cols)
        return;                       /* already large enough */

    seq_t *newA = (seq_t *)malloc(new_rows * new_cols * sizeof (seq_t));
    if (newA == NULL)
        abort();                      /* allocation failure */

    /* fill table */
    for (idx_t r = 0; r < new_rows; ++r) {
        for (idx_t c = 0; c < new_cols; ++c) {
            if (r == 0 && c == 0) {
                newA[0] = (seq_t)1;
            } else if (r == 0) {         /* first row, k ≥ 1 */
                newA[r * new_cols + c] = (seq_t)0;
            } else if (c == 0) {         /* first column, n ≥ 1 */
                newA[r * new_cols + c] = (seq_t)1;
            } else if (c == 1) {         /* use 2^n – 1 recurrence */
                seq_t prev = newA[(r - 1) * new_cols + 1];
                newA[r * new_cols + 1] = prev * (seq_t)2 + (seq_t)1;
            } else {                     /* general recurrence */
                seq_t top    = newA[(r - 1) * new_cols + c];
                seq_t left   = newA[r * new_cols + (c - 1)];
                seq_t diag   = newA[(r - 1) * new_cols + (c - 1)];
                newA[r * new_cols + c] = top * (seq_t)2 + left - diag;
            }
        }
    }

    /* replace old cache */
    free(A_cache);
    A_cache = newA;
    A_rows  = new_rows;
    A_cols  = new_cols;
}

/* ----------  main interface  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)                      /* off‑by‑one index – not used by tests */
        return (seq_t)1;

    /* Find diagonal d  (0‑based) such that d*(d+1)/2 < n ≤ (d+1)*(d+2)/2  */
    uint64_t d = 0;
    while ((d + 1) * (d + 2) / 2 < n)
        ++d;                          /* d < 150 for n up to 10 000 */

    uint64_t before  = d * (d + 1) / 2;          /* terms before diagonal d */
    uint64_t pos_in_d = n - before - 1;           /* 0‑based position in diag */

    idx_t i = (idx_t)pos_in_d;                  /* row index  */
    idx_t j = (idx_t)(d - pos_in_d);           /* column index */

    ensure_table(i, j);                         /* guarantee A_cache large enough */

    return A_cache[i * A_cols + j];
}
