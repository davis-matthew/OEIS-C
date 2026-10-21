/*
 * A054144  Triangular array T: put T(n,0)=n for all n >= 0 and all other
 *          T(n,k)=0; then put T(n,k)=Sum{T(i,j): 0<=j<=i-n+k, n-k<=i<=n}.
 * https://oeis.org/A054144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the triangular array A054144:
 *   T(n,0) = n
 *   T(n,k) = Σ_{t=1..k} T(n‑t, k‑t)  +  Σ_{j=0..k‑1} T(n,j)    (k ≥ 1)
 * The sequence a(n) is the row‑major ordering of T.
 * a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …       */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static storage for computed rows and global sequence */
    static seq_t **row_prefix = NULL;   /* row_prefix[i] points to an array
                                          of prefix sums for row i   */
    static size_t rows_cap   = 0;
    static size_t rows_used  = 0;

    static seq_t *cache      = NULL;    /* flattened a(·) values       */
    static size_t cache_cap  = 0;
    static size_t cache_size = 0;

    /* Grow the array of row prefix pointers if necessary */
    if (rows_used >= rows_cap) {
        size_t new_cap = rows_cap ? rows_cap * 2 : 8;
        seq_t **tmp = (seq_t**)realloc(row_prefix, new_cap * sizeof(seq_t*));
        if (!tmp) abort();
        row_prefix = tmp;
        rows_cap   = new_cap;
    }

    /* Grow the global cache if necessary */
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute rows until we have produced at least indices 0..n */
    while (cache_size <= n) {
        idx_t row = rows_used;                /* current row index   */
        idx_t len = row + 1;                  /* row length         */

        /* Allocate prefix array for this row: prefix[row][k] = Σ_{j=0..k} T(row,j) */
        seq_t *pref = (seq_t*)malloc((size_t)len * sizeof(seq_t));
        if (!pref) abort();

        /* Base value T(row,0) */
        pref[0] = (seq_t)row;
        cache[cache_size++] = pref[0];         /* store a(…) = T(row,0) */

        /* Compute remaining entries of the row */
        for (idx_t k = 1; k <= row; ++k) {
            seq_t sum = 0;                     /* Σ_{t=1..k} T(row‑t, k‑t)  */
            for (idx_t t = 1; t <= k; ++t) {
                seq_t *prev = row_prefix[(size_t)(row - t)];
                sum += prev[k - t];
            }
            sum += pref[k - 1];                 /* Σ_{j=0..k‑1} T(row,j)      */

            pref[k] = pref[k - 1] + sum;         /* store prefix for this row */
            cache[cache_size++] = sum;           /* store a(…) = T(row,k) */
        }

        /* Store pointer to this row's prefix array and advance */
        row_prefix[rows_used] = pref;
        ++rows_used;
    }

    return cache[n];
}
