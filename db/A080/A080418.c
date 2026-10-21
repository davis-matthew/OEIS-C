/*
 * A080418  Generalized Pascal triangle.
 * https://oeis.org/A080418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 179, u32 n <= 612, u64 n <= 2309,
 *            u128 n <= 4950, big any n
 * Helpers:   allocate_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generalized Pascal triangle
 *
 * a(n) enumerates the entries of the triangle T(n,k) defined as
 *   T(1,1) = 1
 *   T(n,1) = 1
 *   T(n,k) = 0  for k > n
 *   T(n,2) = T(n-1,1) + T(n-1,2) + 2·(-1)^n
 *   T(n,k) = T(n-1,k-1) + T(n-1,k) + (-1)^(n+k)   for k > 2
 *
 * The sequence a(n) is the row‑major enumeration of all
 * T(n,k) (for n ≥ 1, 1 ≤ k ≤ n).  The function below builds the
 * triangle incrementally and extracts the required entry.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "../utils/allocate_row.h"

static seq_t **triangle_cache = NULL;      /* triangle_cache[r] points to row r (1‑based) */
static idx_t    cached_rows   = 0;          /* number of rows stored, zero if cache empty */

/* Ensure that the triangle has been computed up to at least row 'needed' */
static void ensure_rows(idx_t needed)
{
    if (cached_rows >= needed)
        return;

    /* Grow the cache array first */
    if (triangle_cache == NULL) {
        triangle_cache = (seq_t **)malloc(sizeof(seq_t *));
        assert(triangle_cache != NULL);
    } else {
        triangle_cache = (seq_t **)realloc(triangle_cache,
                                            (size_t)(needed + 1) * sizeof(seq_t *));
        assert(triangle_cache != NULL);
    }

    /* Compute missing rows */
    for (idx_t r = cached_rows + 1; r <= needed; ++r) {
        seq_t *row = allocate_row(r);
        /* T(r,1) = 1 */
        row[1] = (seq_t)1;

        if (r >= 2) {
            /* T(r,2) = T(r-1,1) + T(r-1,2) + 2*(-1)^r */
            seq_t prev1 = triangle_cache[r-1][1];
            seq_t prev2 = (r-1 >= 2) ? triangle_cache[r-1][2] : (seq_t)0;
            seq_t sum   = prev1 + prev2;
            if ((r & 1ULL) == 0) {
                sum += (seq_t)2;          /* (-1)^r = +1  */
            } else {
                assert(sum >= (seq_t)2);  /* validate underflow cannot occur */
                sum -= (seq_t)2;          /* (-1)^r = -1  */
            }
            row[2] = sum;
        }

        /* Remaining columns */
        for (idx_t k = 3; k <= r; ++k) {
            seq_t p1 = triangle_cache[r-1][k-1];
            seq_t p2 = (k <= r-1) ? triangle_cache[r-1][k] : (seq_t)0;
            seq_t sum = p1 + p2;
            /* Add (-1)^(r+k) */
            if (((r + k) & 1ULL) == 0) { /* even exponent → +1 */
                sum += (seq_t)1;
            } else {                     /* odd exponent  → -1 */
                assert(sum >= (seq_t)1); /* safe underflow check */
                sum -= (seq_t)1;
            }
            row[k] = sum;
        }

        triangle_cache[r] = row;
    }
    cached_rows = needed;
}

/* Find the row r such that the nth element lies in row r
 * (triangular numbers: T(r-1) < n ≤ T(r)) */
static idx_t row_of(idx_t n)
{
    idx_t r = 1;
    /* Compute cumulative size until reaching n */
    idx_t cum = 1;           /* tri(1) */
    while (cum < n) {
        ++r;
        cum += r;            /* cum = tri(r) */
    }
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine which row contains the nth element */
    idx_t r = row_of(n);

    /* Ensure triangle data up to row r is available */
    ensure_rows(r);

    /* Column index within that row */
    idx_t prev_cum = ((idx_t)(r - 1) * (idx_t)r) / 2; /* tri(r-1) */
    idx_t c = n - prev_cum;                           /* 1‑based column */

    return triangle_cache[r][c];
}
