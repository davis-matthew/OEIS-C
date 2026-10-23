/*
 * A135571  Positive integers that are the difference of two positive
 *          triangular numbers in an odd number of ways.
 * https://oeis.org/A135571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates A135571 by merging the sequences of squares, twice‑squares,
   and triangular numbers, and excluding numbers that are both square
   and triangular. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Cache for generated terms */
static seq_t *cache = NULL;        /* array of terms */
static idx_t  cache_size = 0;      /* number of cached terms */
static idx_t  cache_cap  = 0;      /* allocated capacity */

/* Generators for the three base sequences */
static idx_t sq_n   = 2;           /* next n for square (start at 2 → 4) */
static seq_t sq_val = (seq_t)4;    /* current square value */

static idx_t tri_n  = 2;           /* next n for triangular (start at 2 → 3) */
static seq_t tri_val= (seq_t)3;    /* current triangular value */

static idx_t ts_n   = 1;           /* next n for 2*n^2 (start at 1 → 2) */
static seq_t ts_val = (seq_t)2;    /* current twice‑square value */

static bool   last_set   = false;   /* whether last_added has a value */
static seq_t  last_added;           /* last value added to cache */

/* test for a perfect square */
static bool is_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

/* test for a triangular number */
static bool is_triangular(seq_t x)
{
    seq_t v = 8 * x + 1;
    seq_t r = floor_sqrt(v);
    return r * r == v;
}

/* test whether a number is both square and triangular */
static bool is_square_triangular(seq_t x)
{
    return is_square(x) && is_triangular(x);
}

/* Ensure the cache contains at least n terms (n ≥ 1) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;

    /* grow the cache array if needed */
    if (cache_cap < n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
        if (new_cap < n) new_cap = n;
        /* cast for C++ compatibility */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* generate terms up to the requested index */
    while (cache_size < n) {
        /* find the smallest candidate among the three generators */
        seq_t min_val = sq_val;
        if (tri_val < min_val) min_val = tri_val;
        if (ts_val < min_val)  min_val = ts_val;

        /* remember which generators produce this value */
        bool advance_sq = (sq_val == min_val);
        bool advance_tri = (tri_val == min_val);
        bool advance_ts = (ts_val == min_val);

        /* if this value already appeared, skip it */
        if (last_set && min_val == last_added) {
            if (advance_sq) { ++sq_n; sq_val = (seq_t)sq_n * (seq_t)sq_n; }
            if (advance_tri) { ++tri_n; seq_t m = (seq_t)tri_n; tri_val = m * (m + 1) / 2; }
            if (advance_ts) { ++ts_n;  seq_t s = (seq_t)ts_n; ts_val = 2 * s * s; }
            continue;
        }

        /* exclude numbers that are both square and triangular */
        if (is_square_triangular(min_val)) {
            if (advance_sq) { ++sq_n; sq_val = (seq_t)sq_n * (seq_t)sq_n; }
            if (advance_tri) { ++tri_n; seq_t m = (seq_t)tri_n; tri_val = m * (m + 1) / 2; }
            if (advance_ts) { ++ts_n;  seq_t s = (seq_t)ts_n; ts_val = 2 * s * s; }
            /* do not add this value to the cache */
            continue;
        }

        /* accept the value and record it */
        cache[cache_size++] = min_val;
        last_added = min_val;
        last_set = true;

        /* advance generators that produced the accepted value */
        if (advance_sq) { ++sq_n; sq_val = (seq_t)sq_n * (seq_t)sq_n; }
        if (advance_tri) { ++tri_n; seq_t m = (seq_t)tri_n; tri_val = m * (m + 1) / 2; }
        if (advance_ts) { ++ts_n;  seq_t s = (seq_t)ts_n; ts_val = 2 * s * s; }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined; guard */
    ensure_cache(n);
    return cache[n - 1];              /* a(1) is cache[0] */
}
