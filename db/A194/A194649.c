/*
 * A194649  Triangle of coefficients of a sequence of polynomials related to
 *          the enumeration of linear labeled rooted trees.
 * https://oeis.org/A194649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the nth element (row‑major) of the triangle of
 *            coefficients T(n,k) defined by T(0,0) = T(1,0) = 1, T(n+1,k) =
 *            (k+1)*( 2*T(n,k-1) + 3*T(n,k) + T(n,k+1) ) where out‑of‑range
 *            indices are treated as 0. The triangle starts with rows: n=0 : 1
 *            n=1 : 1 n=2 : 3 4 n=3 : 13 36 24 … The sequence a(n)...
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External integer types supplied by the build */
extern seq_t a(idx_t n);

/* ---------------------------------------------------- */
/* Internal memoisation machinery                       */
/* ---------------------------------------------------- */
static seq_t *cache = NULL;            /* cached a(n) values          */
static size_t cache_len = 0;           /* capacity of cache array     */
static idx_t built_up = 0;             /* number of cached elements   */

static seq_t *prev_row = NULL;         /* coefficients of last row computed */
static size_t prev_len = 0;            /* length of prev_row             */
static idx_t last_row = 0;             /* last row number processed      */

/* Ensure cache has at least 'len' slots */
static void ensure_cache_capacity(size_t len)
{
    if (cache_len < len) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * len);
        if (!tmp) abort();
        cache = tmp;
        cache_len = len;
    }
}

/* ---------------------------------------------------- */
/* Public function                                   */
/* ---------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Allocate / initialise the static state on first call */
    if (cache == NULL) {
        /* start with room for at least two values */
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) abort();

        /* row 0 */
        cache[0] = (seq_t)1;
        built_up = 1;

        /* row 1 */
        cache[1] = (seq_t)1;
        built_up = 2;

        /* Initialise previous row to row 1 for recurrence */
        prev_len = 1;
        prev_row = (seq_t *)malloc(sizeof(seq_t));
        if (!prev_row) abort();
        prev_row[0] = (seq_t)1;

        last_row = 1;   /* last computed row number */
    }

    /* If requested value is already cached, return it */
    if (n < built_up) return cache[n];

    /* Generate rows until we have at least n+1 values cached */
    while (built_up <= n) {
        idx_t next_row = last_row + 1;
        size_t len = (next_row >= 2) ? (size_t)next_row : 1;

        /* Allocate array for the next row */
        seq_t *next = (seq_t *)malloc(sizeof(seq_t) * len);
        if (!next) abort();

        /* Compute coefficients of the next row via the recurrence */
        for (size_t k = 0; k < len; ++k) {
            seq_t left  = (k > 0 && k - 1 < prev_len) ? prev_row[k - 1] : 0;
            seq_t mid   = (k < prev_len)               ? prev_row[k]     : 0;
            seq_t right = (k + 1 < prev_len) ? prev_row[k + 1] : 0;
            next[k] = (seq_t)(k + 1) * ( (seq_t)2 * left + (seq_t)3 * mid + right );
        }

        /* Append the new row to the cache */
        ensure_cache_capacity(built_up + len);
        for (size_t k = 0; k < len; ++k) {
            cache[built_up++] = next[k];
        }

        /* Prepare for next iteration */
        free(prev_row);
        prev_row = next;
        prev_len = len;
        last_row = next_row;
    }

    return cache[n];
}
