/*
 * A119727  Triangular array: T(n,k) = T(n,n) = 1, T(n,k) = 5*T(n-1, k-1) +
 *          2*T(n-1, k), read by rows.
 * https://oeis.org/A119727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is obtained by reading the triangular array T defined by
 *            T(n,n) = 1 T(n,k) = 5*T(n-1,k-1) + 2*T(n-1,k) for 1 < k < n The
 *            array is read by rows: row 1, row 2, … . a(n) is the n‑th
 *            element in this row‑wise order. The implementation uses a static
 *            cache of the array values and generates new rows...
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 112, u64 n <= 340,
 *            u128 n <= 1206, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

static seq_t *cache = NULL;          /* triangular array stored as a 1‑D vector */
static idx_t cache_rows = 0;         /* number of rows currently stored (≥0) */

/* Ensure that rows 1..max_rows are available in cache.  New rows are
   computed using the recurrence.  cache is resized if necessary. */
static void ensure_rows(idx_t max_rows) {
    if (max_rows <= cache_rows) return;

    /* current size of cache: (cache_rows)*(cache_rows+1)/2 elements */
    idx_t old_elements = cache_rows * (cache_rows + 1) / 2;
    /* new size after adding rows: (max_rows)*(max_rows+1)/2 elements */
    idx_t new_elements = max_rows * (max_rows + 1) / 2;

    /* reallocate memory */
    seq_t *new_cache = cache;
    if (new_cache == NULL) {
        new_cache = (seq_t*)malloc(sizeof(seq_t) * new_elements);
    } else {
        new_cache = (seq_t*)realloc(new_cache, sizeof(seq_t) * new_elements);
    }
    if (new_cache == NULL) abort();  /* allocation failed: terminate */
    cache = new_cache;

    /* compute newly required rows */
    for (idx_t r = cache_rows + 1; r <= max_rows; ++r) {
        idx_t base = r * (r - 1) / 2;      /* offset of first element in row r */
        idx_t prev_base = (r - 1) * (r - 2) / 2; /* first element offset of previous row */
        cache[base] = (seq_t)1; /* T(r,1) = 1 */

        /* interior elements */
        for (idx_t k = 2; k < r; ++k) {
            /* indices of T(r-1,k-1) and T(r-1,k) */
            idx_t idx_prev_km1 = prev_base + (k - 2); /* k-1 in previous row */
            idx_t idx_prev_k   = prev_base + (k - 1); /* k   in previous row */
            cache[base + k - 1] = (seq_t)5 * cache[idx_prev_km1]
                               + (seq_t)2 * cache[idx_prev_k];
        }

        cache[base + r - 1] = (seq_t)1; /* T(r,r) = 1 */
    }
    cache_rows = max_rows;
}

/* Convert a global index n (1‑based) into row r and column k
   according to the flattened row‑wise order. */
static void index_to_rc(idx_t n, idx_t *r_out, idx_t *k_out) {
    if (n == 0) {
        *r_out = 0;
        *k_out = 0;
        return;
    }
    double d = sqrt((double)8 * n + 1);
    idx_t r = (idx_t)((d - 1) / 2);          /* floor of the formula */
    /* adjust r to be minimal with r*(r+1)/2 >= n */
    if (r * (r + 1) / 2 < n) ++r;
    while ((r - 1) * r / 2 >= n) --r;      /* safety in case of overflow */
    idx_t prev = r * (r - 1) / 2;           /* cumulative count before row r */
    idx_t k = n - prev;                     /* 1‑based column within row r */
    *r_out = r;
    *k_out = k;
}

seq_t a(idx_t n) {
    if (n == 0) {
        /* The sequence is defined for n ≥ 1; return 0 for n == 0 */
        return (seq_t)0;
    }

    idx_t r, k;
    index_to_rc(n, &r, &k);
    ensure_rows(r);

    idx_t offset = r * (r - 1) / 2 + (k - 1);
    return cache[offset];
}
