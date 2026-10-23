/*
 * A116575  a(0) = 0; a(1) = a(2) = 1; for n > 2: a(n) = a(a(n-2)) + a(n-1 -
 *          abs(a(n-1) - a(n-2))) if a(n-2) < n, a(n) = 0 otherwise.
 * https://oeis.org/A116575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, less
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A116575 – “Limited chaotic sequence”
 *
 *  Recurrence:
 *      a(0) = 0;
 *      a(1) = a(2) = 1;
 *      for n > 2:
 *          if a(n-2) < n
 *              a(n) = a(a(n-2)) + a(n-1 - |a(n-1)-a(n-2)|)
 *          else
 *              a(n) = 0
 *
 *  The function memoises previously computed terms to allow
 *  arbitrary ordering of calls.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of stored terms */

/* enlarge the cache to hold at least ‘need’ entries */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* allocation failure – abort as required */
    cache = tmp;
    cache_cap = new_cap;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* base values */
    if (n == 0)
        return 0;
    if (n == 1 || n == 2)
        return 1;

    /* initialise cache on first use */
    if (cache_len == 0) {
        ensure_capacity(3);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache_len = 3;
    }

    /* if already computed, return cached value */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* ensure space for all new indices */
    ensure_capacity((size_t)n + 1);

    /* compute values from next missing index up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t a_im2 = cache[i - 2];
        seq_t val;

        if (a_im2 < (seq_t)i) {   /* condition holds */
            seq_t term1 = cache[(size_t)a_im2];

            /* absolute difference of a(i-1) and a(i-2) */
            seq_t diff = (cache[i - 1] > cache[i - 2])
                         ? (cache[i - 1] - cache[i - 2])
                         : (cache[i - 2] - cache[i - 1]);

            seq_t term2 = 0;
            if (diff <= (seq_t)(i - 1)) {
                /* compute second index safely */
                idx_t idx2 = (i - 1) - (idx_t)diff;
                term2 = cache[(size_t)idx2];
            }
            /* otherwise term2 remains 0 (should not occur) */

            val = term1 + term2;
        } else {
            val = 0;
        }

        cache[(size_t)i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
