/*
 * A202399  Number of (n+2)X3 binary arrays avoiding patterns 000 and 010 in
 *          rows and columns.
 * https://oeis.org/A202399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A202399: Number of (n+2)×3 binary arrays avoiding patterns 000 and 010
   in rows and columns.  Empirical linear recurrence of order 14:
      a(1)  = 126
      a(2)  = 441
      a(3)  = 1785
      a(4)  = 7225
      a(5)  = 27880
      a(6)  = 107584
      a(7)  = 422136
      a(8)  = 1656369
      a(9)  = 6459453
      a(10) = 25190361
      a(11) = 98462742
      a(12) = 384865924
      a(13) = 1503033070
      a(14) = 5869858225
   For n  > 14:
      a(n) = 2*a(n-1) - a(n-2) + 18*a(n-3) + 44*a(n-4) + 38*a(n-5)
             + 80*a(n-6) - 80*a(n-8) - 38*a(n-9) - 44*a(n-10)
             - 18*a(n-11) + a(n-12) - 2*a(n-13) + a(n-14). */

#include <stdlib.h>
#include <stddef.h>

/* Initial values (indices 1‑14).  init[0] is unused. */
static const seq_t init[15] = {
    0ULL,                         /* 0 unused */
    126ULL,                       /* a(1) */
    441ULL,                       /* a(2) */
    1785ULL,                      /* a(3) */
    7225ULL,                      /* a(4) */
    27880ULL,                     /* a(5) */
    107584ULL,                    /* a(6) */
    422136ULL,                    /* a(7) */
    1656369ULL,                   /* a(8) */
    6459453ULL,                   /* a(9) */
    25190361ULL,                  /* a(10) */
    98462742ULL,                  /* a(11) */
    384865924ULL,                 /* a(12) */
    1503033070ULL,                /* a(13) */
    5869858225ULL                 /* a(14) */
};

/* Static buffer for memoisation */
static seq_t *cache = NULL;      /* pointer to buffer of seq_t */
static size_t cache_cap = 0;     /* current capacity of buffer */
static size_t cache_len = 0;     /* highest index computed (>=14 after init) */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined only for n ≥ 1; return 0 as a safe fallback. */
        return 0;
    }
    size_t idx = (size_t)n;          /* n fits into size_t (uint64_t) */

    /* Handle the first 14 terms directly. */
    if (idx <= 14) {
        return init[idx];
    }

    /* Grow the cache if necessary.  We need capacity >= idx+1. */
    if (cache_cap < idx + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < idx + 1) {
            new_cap = idx + 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Memory allocation failed; return 0 as a fallback. */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
        /* Ensure cache[0] stays defined for safety. */
        if (cache_len == 0) {
            cache[0] = 0;
        }
    }

    /* Initialise the first 14 entries if not already done. */
    if (cache_len < 14) {
        for (size_t i = 1; i <= 14; ++i) {
            cache[i] = init[i];
        }
        cache_len = 14;
    }

    /* Compute all values up to idx that are still missing. */
    for (size_t i = cache_len + 1; i <= idx; ++i) {
        seq_t pos = 2 * cache[i - 1]      /* +2*a(i-1) */
                  + 18 * cache[i - 3]     /* +18*a(i-3) */
                  + 44 * cache[i - 4]     /* +44*a(i-4) */
                  + 38 * cache[i - 5]     /* +38*a(i-5) */
                  + 80 * cache[i - 6]     /* +80*a(i-6) */
                  + cache[i - 12]         /* +a(i-12)   */
                  + cache[i - 14];        /* +a(i-14)   */

        seq_t neg = cache[i - 2]          /* -a(i-2)   */
                  + 80 * cache[i - 8]     /* +80*a(i-8) */
                  + 38 * cache[i - 9]     /* +38*a(i-9) */
                  + 44 * cache[i - 10]    /* +44*a(i-10) */
                  + 18 * cache[i - 11]    /* +18*a(i-11) */
                  + 2 * cache[i - 13];    /* +2*a(i-13) */

        cache[i] = pos - neg;
    }

    cache_len = idx;
    return cache[idx];
}
