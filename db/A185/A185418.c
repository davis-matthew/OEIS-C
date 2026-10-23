/*
 * A185418  Square array, read by antidiagonals, used to recursively calculate
 *          the Springer numbers A001586.
 * https://oeis.org/A185418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A185418:  square array read by antidiagonals.
   T(0,k)=1 for all k.
   Recurrence:  T(n+1,k) = k*T(n,k-1)+(k+1)*T(n,k+1)   (n,k≥0)
   The sequence a(n) is obtained by reading T(n,k) along the
   antidiagonals: for each d≥0, output T(n,k) with n+d-k = d, i.e.
   (i,j) with i+j=d in order n=d,d-1,…,0.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* cache for computed T(n,k) values */
static seq_t   *cache_data = NULL; /* flat array */
static bool    *cache_flag = NULL;
static idx_t    cache_rows = 0;    /* maximum stored row index (inclusive)   */
static idx_t    cache_cols = 0;    /* maximum stored column index (inclusive) */

/* enlarge cache to contain all rows up to max_row and columns up to max_col. */
static void ensure_cache(idx_t max_row)
{
    if (max_row <= cache_rows)
        return;                       /* already large enough */

    /* new limits: rows 0..max_row, columns 0..max_col where max_col = max_row */
    idx_t new_rows = max_row + 1;
    idx_t new_cols = max_row;          /* column indices 0..new_cols inclusive */

    size_t new_size = (size_t)new_rows * (new_cols + 1);

    seq_t  *new_data = (seq_t *)malloc(new_size * sizeof(seq_t));
    bool   *new_flag = (bool  *)malloc(new_size * sizeof(bool));

    if (!new_data || !new_flag) {
        /* memory exhaustion */
        exit(1);
    }

    /* zero the new flag array */
    for (size_t i = 0; i < new_size; ++i)
        new_flag[i] = false;

    /* copy old data */
    if (cache_data) {
        for (idx_t r = 0; r <= cache_rows; ++r) {
            for (idx_t c = 0; c <= cache_cols; ++c) {
                size_t old_idx = (size_t)r * (cache_cols + 1) + c;
                size_t new_idx = (size_t)r * (new_cols + 1) + c;
                new_data[new_idx] = cache_data[old_idx];
                new_flag[new_idx] = cache_flag[old_idx];
            }
        }
    }

    /* free old memory */
    free(cache_data);
    free(cache_flag);

    cache_data  = new_data;
    cache_flag  = new_flag;
    cache_rows  = max_row;
    cache_cols  = new_cols;            /* max column index after expansion */
}

/* recursive evaluation with memoisation */
static seq_t T_value(idx_t n, idx_t k)
{
    if (n == 0)
        return (seq_t)1;              /* T(0,k)=1 for all k */

    size_t idx = (size_t)n * (cache_cols + 1) + k;
    if (cache_flag[idx])
        return cache_data[idx];

    seq_t left  = (k == 0) ? (seq_t)0 : T_value(n - 1, k - 1);
    seq_t right = T_value(n - 1, k + 1);

    seq_t result = (seq_t)k * left + (seq_t)(k + 1) * right;

    cache_flag[idx] = true;
    cache_data[idx] = result;
    return result;
}

/* public interface */
seq_t a(idx_t n)
{
    /* determine diagonal d where a(n) lies: d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    idx_t offset = n - d * (d + 1) / 2;     /* 0‑based position in diagonal d */
    idx_t i = d - offset;                   /* row index   */
    idx_t j = offset;                       /* column index */

    ensure_cache(d);                        /* cache at least all rows up to d */
    return T_value(i, j);
}
