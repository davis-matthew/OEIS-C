/*
 * A063292  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          19 ).
 * https://oeis.org/A063292
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13 with initial values:
 *            a(0) = 0, a(1) = 0, a(2) = -1, a(3) = 7, a(4) = 21, a(5) = 36,
 *            a(6) = 51, a(7) = 66, a(8) = 81, a(9) = 96, a(10) = 111, a(11) =
 *            126, a(12) = 141, a(13) = 154
 * Range:     i8 n <= 11, i16 n <= 51, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>   /* for memcpy */

static seq_t *cache = NULL;        /* dynamic array of computed terms */
static size_t cache_cap = 0;       /* allocated capacity */
static size_t cache_len = 0;       /* number of entries already filled */

/* constant base values up to n = 13 */
static const seq_t base_vals[14] = {
    0,   /* a(0) – unused */
    0,   /* a(1) – unused */
   -1,   /* a(2) */
    7,   /* a(3) */
   21,   /* a(4) */
   36,   /* a(5) */
   51,   /* a(6) */
   66,   /* a(7) */
   81,   /* a(8) */
   96,   /* a(9) */
  111,   /* a(10) */
  126,   /* a(11) */
  141,   /* a(12) */
  154    /* a(13) */
};

/* Ensure cache contains values up to n (inclusive). */
static void ensure_cache_up_to(idx_t n)
{
    /* Lazy allocation on first call */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;     /* nothing filled yet */
    }

    /* Expand storage if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms in increasing order */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i <= 13) {
            val = base_vals[i];
        } else {
            val = cache[i - 4] + cache[i - 6] - cache[i - 10];
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    if (n < 2) {
        /* Behaviour for n < offset is unspecified; return 0. */
        return (seq_t)0;
    }
    ensure_cache_up_to(n);
    return cache[(size_t)n];
}
