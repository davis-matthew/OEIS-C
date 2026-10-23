/*
 * A129577  Triangle, read by rows, defined by T(n,k) = T(n-1,k) + T(n,k-1)
 *          for nk>0, where T(n,0) = T(n-1,0) + T(n-1,n-1) and T(n,n) =
 *          T(n,n-1) for n>0 with T(0,0)=1.
 * https://oeis.org/A129577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A129577: Triangle read by rows.  The entry a(n) is the n‑th
   entry of the triangular array defined by
     T(0,0)   = 1
     T(1,0)   = 1,  T(1,1) = 1          (first two rows explicit)
     T(n,0)   = T(n-1,0) + T(n-1,n-1)   (n >= 2)
     T(n,n)   = T(n,n-1)                (n >= 1)
     T(n,k)   = T(n-1,k) + T(n,k-1)     (1 <= k < n, n >= 2)
   The function below computes a(n) for any n >= 0, using a memoised
   generation of the rows.  It is O(n) time and O(n) storage, and works
   for any integer type seq_t supplied by the build.  */
/* Implementation notes:
   * The cache holds all previous terms in flattened order.
   * The last generated row is kept in 'prev' to build the next row.
   * For the first two rows (n = 0 and 1) we initialise explicitly,
     because the recurrence rules differ there. */

#include <stdlib.h>

/* Ensure the cache can hold indices 0..need-1. */
static void ensure_cache_cap(seq_t **p, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
    *p = tmp;
    *cap = new_cap;
}

/* Return the n‑th term of A129577. */
seq_t a(idx_t n)
{
    /* Static state */
    static seq_t *cache = NULL;      /* flattened triangle */
    static size_t cache_len = 0;     /* number of elements stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    static seq_t *prev = NULL;      /* previous row (row r-1) */
    static size_t prev_len = 0;     /* length of previous row */
    static idx_t processed_row = -1;/* last processed row index (0‑based) */

    /* Initialise on first call: rows 0 and 1 explicitly. */
    if (cache == NULL) {
        /* Row 0: [1] */
        ensure_cache_cap(&cache, &cache_cap, 3);
        cache[0] = (seq_t)1;
        /* Row 1: [1,1] */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_len = 3;
        processed_row = 1;

        /* Allocate previous row (= row 1) for future generation */
        prev_len = 2;
        prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
        prev[0] = (seq_t)1;
        prev[1] = (seq_t)1;
    }

    /* Generate rows until we have the n‑th element */
    while (cache_len <= (size_t)n) {
        /* Next row number */
        idx_t r = processed_row + 1;
        size_t len = (size_t)r + 1;           /* number of entries in this row */

        /* Allocate current row */
        seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));

        /* Compute first element */
        curr[0] = prev[0] + prev[prev_len - 1];   /* prev_len = r */

        /* Compute middle elements (1 <= k < r) using recurrence */
        for (size_t k = 1; k < r; ++k) {
            curr[k] = prev[k] + curr[k - 1];
        }

        /* Last element */
        curr[r] = curr[r - 1];

        /* Append this row to the cache */
        for (size_t k = 0; k < len; ++k) {
            ensure_cache_cap(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = curr[k];
        }

        /* Prepare for next iteration */
        free(prev);
        prev = curr;
        prev_len = len;
        processed_row = r;
    }

    return cache[(size_t)n];
}
