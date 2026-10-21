/*
 * A210204  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210203; see the Formula section.
 * https://oeis.org/A210204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A210204 – triangle of coefficients of the polynomials v(n,x).
 *            Recurrence (OEIS): u(1,x)=1, v(1,x)=1 u(n,x)=u(n-1,x)+v(n-1,x)+1
 *            v(n,x)=(x+1)*u(n-1,x)+(x+1)*v(n-1,x)+1 The sequence a(n) is the
 *            flattened list of the entries of v(n,x)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static idx_t max_computed_row = 0;     /* rows already processed (1‑based) */
static seq_t *prev_u = NULL;           /* u array of last processed row */
static seq_t *prev_v = NULL;           /* v array of last processed row */
static seq_t **v_storage = NULL;       /* v(n,x) coefficients for each row */
static size_t v_storage_size = 0;      /* allocated slots in v_storage */

/* Ensure the pointer array v_storage has space up to index 'need'.
   'need' is zero‑based (row-1). */
static void ensure_v_storage(idx_t need)
{
    if ((size_t)need < v_storage_size)
        return;
    size_t new_size = v_storage_size ? v_storage_size : 1;
    while (new_size <= (size_t)need)
        new_size <<= 1;
    void *tmp = realloc(v_storage, new_size * sizeof(seq_t *));
    if (!tmp)
        exit(1);
    v_storage = (seq_t **)tmp;
    v_storage_size = new_size;
}

/* Find the row k such that the first k(k+1)/2 entries cover at least n. */
static idx_t row_of_index(idx_t n)
{
    idx_t lo = 1, hi = 1;
    while (hi * (hi + 1) / 2 < n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Compute all rows up to and including 'target_row' (1‑based). */
static void compute_rows_up_to(idx_t target_row)
{
    for (idx_t row = max_computed_row + 1; row <= target_row; ++row) {
        idx_t len = row;                       /* number of coefficients */
        seq_t *curr_u = seq_alloc_new_array(len);
        seq_t *curr_v = seq_alloc_new_array(len);

        if (row == 1) {
            curr_u[0] = 1;
            curr_v[0] = 1;
        } else {
            idx_t prev_len = len - 1;
            for (idx_t k = 0; k < prev_len; ++k) {
                seq_t sum = prev_u[k] + prev_v[k];
                curr_u[k] = sum;               /* +1 handled below */
                curr_v[k]   += sum;            /* unchanged degree */
                curr_v[k+1] += sum;            /* shifted (×x) */
            }
            curr_u[0] = prev_u[0] + prev_v[0] + 1;
            curr_v[0] += 1;                    /* +1 in recurrence */
        }

        /* Store the computed v(n,x) for later retrieval */
        ensure_v_storage(row - 1);
        v_storage[row - 1] = curr_v;

        /* Free the old u array and update pointers */
        seq_t *old_u = prev_u;
        prev_u = curr_u;
        prev_v = curr_v;
        if (old_u)
            seq_free_array(old_u);

        max_computed_row = row;
    }
}

/* Public function requested by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for indices below the offset */

    idx_t row = row_of_index(n);

    if (row > max_computed_row)
        compute_rows_up_to(row);

    idx_t prev_count = (row - 1) * row / 2;     /* entries before this row */
    idx_t offset    = n - prev_count;          /* 1‑based position in row */
    idx_t deg       = offset - 1;              /* coefficient index (0‑based) */

    return v_storage[row - 1][deg];
}
