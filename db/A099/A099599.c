/*
 * A099599  Triangle T read by rows: coefficients of polynomials generating
 *          array A099597.
 * https://oeis.org/A099599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 21, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A099599 – Flattened coefficients of the triangle T.
 *  Recurrence for the triangle:
 *      T(0,0) = 1,                     T(1,0) = 1, T(1,1) = 1
 *      For n ≥ 2 and k > 0:
 *          T(n,k) = n * ( (n-1)*(T(n-2,k) - T(n-1,k)) + T(n-1,k-1) )
 *      (Values outside the row are treated as 0.)
 *
 *  The sequence a(n) is obtained by reading the triangle row‑by‑row,
 *  flattening it into a one‑dimensional array starting with 0.
 *
 *  This implementation builds the triangle incrementally, caching the
 *  flattened values in a growing array so that repeated calls to a(n)
 *  are O(1) after a precomputation up to the largest requested index.
 *
 *  All arithmetic is performed in the type seq_t, which may be a
 *  signed integer type (int8_t … int64_t, __int128) or an arbitrary‑
 *  precision integer type.  Intermediate results are kept within the
 *  same type; if a value fits in seq_t it will be computed exactly.
 */

#include <stdlib.h>

/* State for incremental generation of the triangle values */
static seq_t *flat_cache = NULL;          /* flattened sequence a(n)   */
static idx_t  cache_cap  = 0;             /* capacity of flat_cache   */
static idx_t  cache_len  = 0;             /* number of values stored  */

static seq_t *prev = NULL;                /* row (n‑1)                */
static seq_t *prev2 = NULL;               /* row (n‑2)                */
static idx_t  len_prev  = 0;              /* length of prev row       */
static idx_t  len_prev2 = 0;              /* length of prev2 row      */
static idx_t  next_row  = 0;              /* next row index to compute (0‑based) */

/* Ensure the flattened cache has space for at least need elements. */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(flat_cache, new_cap * sizeof(seq_t));
    flat_cache = tmp;
    cache_cap  = new_cap;
}

/* Append a value to the flattened cache. */
static void push_cache(seq_t val)
{
    if (cache_len == cache_cap)
        grow_cache(cache_len + 1);
    flat_cache[cache_len++] = val;
}

/* Compute the next row of the triangle and append its values to the cache. */
static void compute_next_row(void)
{
    idx_t r = next_row;   /* current row index */

    if (r == 0) { /* Row 0: [1] */
        seq_t *cur = (seq_t *)malloc(sizeof(seq_t));
        cur[0] = (seq_t)1;
        push_cache(cur[0]);

        /* Update state: prev becomes this row, prev2 stays NULL */
        if (prev)
            free(prev);  /* Should be NULL on first call */
        prev = cur;
        len_prev  = 1;
        len_prev2 = 0;
        prev2 = NULL;
    }
    else if (r == 1) { /* Row 1: [1, 1] */
        seq_t *cur = (seq_t *)malloc(2 * sizeof(seq_t));
        cur[0] = (seq_t)1;
        cur[1] = (seq_t)1;
        push_cache(cur[0]);
        push_cache(cur[1]);

        /* Shift rows: old prev (row 0) becomes prev2 */
        if (prev2)
            free(prev2);          /* prev2 is NULL here */
        prev2 = prev;            /* row 0 */
        prev  = cur;             /* row 1 */
        len_prev2 = len_prev;    /* 1 */
        len_prev  = 2;
    }
    else { /* r >= 2 */
        idx_t n = r;
        idx_t row_len = n + 1;
        seq_t *cur = (seq_t *)malloc(row_len * sizeof(seq_t));

        /* k = 0 value is always 1 */
        cur[0] = (seq_t)1;

        /* Compute k = 1 .. n using the recurrence */
        for (idx_t k = 1; k <= n; ++k) {
            /* diff = (n-1) * (T(n-2,k) - T(n-1,k)) */
            seq_t diff = 0;
            if (k < len_prev2)
                diff += prev2[k];
            if (k < len_prev)
                diff -= prev[k];
            diff = (seq_t)(n - 1) * diff;

            /* add = T(n-1,k-1) */
            seq_t add = 0;
            if (k - 1 < len_prev)
                add += prev[k - 1];

            cur[k] = (seq_t)n * (diff + add);
        }

        /* Append current row to the flattened cache */
        for (idx_t k = 0; k < row_len; ++k)
            push_cache(cur[k]);

        /* Shift rows for next iteration: old prev2 is no longer needed. */
        if (prev2)
            free(prev2);     /* free row (n-2) */
        prev2 = prev;        /* previous row becomes prev2   */
        prev  = cur;         /* current row becomes prev     */
        len_prev2 = len_prev;/* len_prev was length of row (n-1) */
        len_prev  = row_len; /* new row length */
    }

    next_row++; /* advance to the next row next time */
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    /* Allocate state on first call. */
    if (!flat_cache) {
        cache_cap = 4;
        flat_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_row = 0;
        prev = prev2 = NULL;
        len_prev = len_prev2 = 0;
    }

    /* Generate rows until the cache contains a(n). */
    while (cache_len <= n) {
        compute_next_row();
    }

    return flat_cache[n];
}
