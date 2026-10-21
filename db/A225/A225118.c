/*
 * A225118  Triangle read by rows, coefficients of the generalized Eulerian
 *          polynomials A_{n, 4}(x) in descending order.
 * https://oeis.org/A225118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225118: triangle read by rows, coefficients of generalized
 * Eulerian polynomials A_{n,4}(x) in descending order.
 *
 * T(n, k) satisfies the recurrence:
 *   T(0, 0) = 1,
 *   T(n, -1) = 0,
 *   T(n, k) = 0 for n < k,
 *   T(n, k) = (4*(n-k)+1)*T(n-1,k-1) + (3+4*k)*T(n-1,k)  (n ≥ 1, 0 ≤ k ≤ n)
 *
 * a(n) is the n-th element in the flattened triangle. That is,
 * for the unique r with (r+1)(r+2)/2 > n,
 *   a(n) = T(r, n - r*(r+1)/2).
 *
 * The implementation caches rows of the triangle so that each row is
 * computed only once, even when calls are in arbitrary order.
 *
 * All arithmetic is performed using seq_t, which may be a built‑in
 * unsigned integer or an arbitrary‑precision integer class.
 *
 * Note: the build supplies the helper function seq_alloc(size_t) for
 * allocating arrays of seq_t.  Standard library functions are used
 * for the row pointer array.  No floating point or <math.h> is used.
 */

#include <stddef.h>          /* for size_t */
#include <stdint.h>          /* for uint64_t */
#include <stdlib.h>          /* for malloc / realloc */
#include "../utils/seq_alloc.h"   /* for seq_alloc(size_t) */

static seq_t **row_cache = NULL;   /* array of pointers to each row   */
static size_t  cache_len = 0;      /* number of rows currently stored */
static size_t  cache_cap = 0;      /* capacity of row_cache array     */

/* Ensure that rows 0..r are computed and stored in row_cache. */
static void ensure_rows_upto(idx_t r)
{
    if (cache_len > r) return;                 /* already available */

    /* Grow the row_cache pointer array if necessary */
    if (r >= cache_cap) {
        size_t new_cap = r + 1;
        seq_t **tmp = (seq_t **)realloc(row_cache, new_cap * sizeof(seq_t *));
        if (!tmp) abort();                     /* out of memory */
        row_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing rows one by one */
    for (idx_t n = cache_len; n <= r; ++n) {
        size_t len = n + 1;                     /* number of elements in row n */
        seq_t *row = seq_alloc(len);            /* allocate row array */
        if (!row) abort();

        if (n == 0) {
            /* Base row: T(0,0) = 1 */
            row[0] = (seq_t)1;
        } else {
            seq_t *prev = row_cache[n - 1];     /* previous row */
            for (idx_t k = 0; k <= n; ++k) {
                seq_t val = 0;
                /* term from T(n-1, k-1) if k > 0 */
                if (k > 0) {
                    seq_t factor1 = ((seq_t)4) * ((seq_t)(n - k)) + (seq_t)1;
                    val = val + prev[k - 1] * factor1;
                }
                /* term from T(n-1, k) if k <= n-1 */
                if (k <= n - 1) {
                    seq_t factor2 = (seq_t)3 + ((seq_t)4) * ((seq_t)k);
                    val = val + prev[k] * factor2;
                }
                row[k] = val;
            }
        }
        row_cache[n] = row;
    }

    cache_len = r + 1;
}

/* Compute the nth term of sequence A225118.  The sequence is
 * indexed starting at 0, matching the OEIS offset. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;             /* handled directly */

    /* Find the row r such that (r+1)(r+2)/2 > n */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;

    /* Position within the row (zero‑based) */
    idx_t pos = n - (r * (r + 1)) / 2;

    /* Ensure rows up to r are computed */
    ensure_rows_upto(r);

    return row_cache[r][pos];
}
