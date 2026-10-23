/*
 * A124572  Triangle read by rows where the n-th row is the first row of M^n,
 *          with M the (n+1)-by-(n+1) matrix with (1,3,1,3,1,3,...) on its
 *          main diagonal and (3,1,3,1,3,1,...) on its superdiagonal.
 * https://oeis.org/A124572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 159, u64 n <= 603,
 *            u128 n <= 2231, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows where the n‑th row is the first row of M^n,
 * with M the (n+1)-by-(n+1) matrix having (1,3,1,3,…) on its main
 * diagonal and (3,1,3,1,…) on its superdiagonal.
 *
 * Flattened sequence a(n) is obtained by concatenating all rows:
 *   a(n) = first_row_of_M^r[c]   where
 *   r = largest integer with r(r+1)/2 <= n,
 *   c = n - r(r+1)/2          (0 <= c <= r).
 *
 * The algorithm builds each required row by repeated multiplication
 * of the row vector by its (r+1)-by-(r+1) matrix, using only small
 * constants (1 or 3), which keeps intermediate values small.
 *
 * This implementation uses dynamic caching so that any order of
 * calls to a(idx_t) is supported efficiently.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* -------------------------- cache of flattened triangle -------------------------- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of elements stored so far   */
static size_t cache_cap  = 0;   /* capacity of the cache array         */

/* Append an element to the cache, growing its capacity if needed */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = alloc_seq_array(new_cap);
        for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
        if (cache) free_seq_array(cache);
        cache     = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* --------------------------- compute a single row ----------------------------- */
static void compute_and_append_row(idx_t r)
{
    size_t len = (size_t)r + 1;
    seq_t *row   = alloc_seq_array(len);
    seq_t *temp  = alloc_seq_array(len);

    /* initialise with the first row of M_r */
    for (size_t i = 0; i < len; ++i) row[i] = 0;
    row[0] = (seq_t)1;
    if (r >= 1) row[1] = (seq_t)3;

    /* repeated multiplication: row = row * M_r, r-1 times */
    size_t max_idx = (r >= 1) ? 1 : 0;        /* largest index that may be non‑zero so far */
    for (idx_t step = 1; step < r; ++step) {
        size_t new_max = (size_t)step + 1;    /* after this multiplication max index becomes step+1 */

        /* element 0: only diagonal contributes */
        temp[0] = row[0];                    /* diag[0] = 1 */

        for (size_t j = 1; j <= new_max; ++j) {
            seq_t val = 0;
            /* contribution from row[j] * diag[j]  (if j <= max_idx) */
            if (j <= max_idx) {
                seq_t d = ((j & 1) == 0) ? (seq_t)1 : (seq_t)3;  /* diag[j] */
                val += row[j] * d;
            }
            /* contribution from row[j-1] * superdiag[j-1]  (if j-1 <= max_idx) */
            if (j > 0 && (j - 1) <= max_idx) {
                seq_t s = (((j - 1) & 1) == 0) ? (seq_t)3 : (seq_t)1; /* superdiag[j-1] */
                val += row[j - 1] * s;
            }
            temp[j] = val;
        }
        /* remaining elements beyond new_max are already zero */

        /* swap buffers */
        seq_t *old = row;
        row = temp;
        temp = old;
        max_idx = new_max;
    }

    /* append the computed row to the flattened cache */
    for (size_t i = 0; i < len; ++i)
        cache_append(row[i]);

    free_seq_array(temp);
    free_seq_array(row);
}

/* ---------------------------- main entry point ------------------------------- */
static idx_t next_row_to_compute = 1;   /* next row index yet to be built */

seq_t a(idx_t n)
{
    /* initialise if this is the first call */
    if (cache_len == 0) {
        compute_and_append_row(0);        /* build row 0 */
        next_row_to_compute = 1;
    }

    /* build rows until we cover index n */
    while (cache_len <= (size_t)n) {
        compute_and_append_row(next_row_to_compute);
        ++next_row_to_compute;
    }

    return cache[(size_t)n];
}
