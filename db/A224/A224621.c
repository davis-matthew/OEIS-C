/*
 * A224621  Number of (n+4) X 8 0..2 matrices with each 5 X 5 subblock
 *          idempotent.
 * https://oeis.org/A224621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A224621 – Number of (n+4)×8 0..2 matrices with each 5×5 subblock idempotent.
   Empirical linear recurrence for n > 13:
     a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 3*a(n-4)
            - 2*a(n-6) + 2*a(n-8) - a(n-9)
   Initial values (1 ≤ n ≤ 13):
     a(1)  = 32029
     a(2)  = 22081
     a(3)  = 26624
     a(4)  = 29254
     a(5)  = 30723
     a(6)  = 31623
     a(7)  = 39450
     a(8)  = 51956
     a(9)  = 66929
     a(10) = 83137
     a(11) = 100012
     a(12) = 124474
     a(13) = 161207
   The function memoises previously‑computed terms so that repeated calls have
   amortised O(1) cost.  It works for any seq_t and idx_t types supplied by
   the build configuration. */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, realloc, abort */

/* Prefix values for n = 1 … 13.  Index 0 is unused (value 0). */
static const seq_t pre[14] = {
    (seq_t)0,
    (seq_t)32029,
    (seq_t)22081,
    (seq_t)26624,
    (seq_t)29254,
    (seq_t)30723,
    (seq_t)31623,
    (seq_t)39450,
    (seq_t)51956,
    (seq_t)66929,
    (seq_t)83137,
    (seq_t)100012,
    (seq_t)124474,
    (seq_t)161207
};

/* Memoisation cache – dynamically grown on demand. */
static seq_t *cache = NULL;     /* pointer to array holding a(0) … a(k) */
static size_t cache_cap = 0;    /* currently allocated capacity (size_t) */
static size_t cache_len = 0;    /* number of valid entries in cache */

/* Ensure the cache has space for at least 'need' elements (0 … need-1). */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = need * 2;
        if (new_cap < 16) new_cap = 16;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();            /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Undefined for n < 1.  Return 0 for safety. */
    if (n < 1) return (seq_t)0;

    /* Handle the initial 13 terms directly. */
    if (n <= 13) return pre[(size_t)n];

    /* Initialise cache on first use.  Preserve the known prefix. */
    if (!cache) {
        size_t init_cap = (size_t)n + 1;
        if (init_cap < 14) init_cap = 14;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();  /* out of memory */
        /* Copy known prefix. */
        for (size_t i = 0; i <= 13; ++i) cache[i] = pre[i];
        cache_cap = init_cap;
        cache_len = 14;            /* entries 0 … 13 are valid */
    }

    /* Extend the cache if the requested n is beyond the current length. */
    if ((size_t)n >= cache_len) {
        /* Need space for indices 0 … n. */
        ensure_cache_capacity((size_t)n + 1);
        /* Compute missing terms using the recurrence. */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* All arithmetic is performed with seq_t.  The recurrence is
               guaranteed to stay non‑negative for the valid sequence. */
            seq_t sum1 = cache[i-1] * 3 + cache[i-4] * 3 + cache[i-8] * 2;
            seq_t sum2 = cache[i-2] * 2 + cache[i-3] * 2 + cache[i-6] * 2 + cache[i-9];
            cache[i]   = sum1 - sum2;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
