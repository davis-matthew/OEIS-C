/*
 * A039027  Numbers whose base-6 representation has the same number of 3's and
 *          4's.
 * https://oeis.org/A039027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th number whose base‑6 representation has equal number
 *            of 3's and 4's (OEIS A039027)
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */ 
#include <stdbool.h>
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache      = NULL;   /* array of found terms */
static idx_t cache_size  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity of cache */
static seq_t next_candidate = 0;   /* next number to test */

/* ------------------------------------------------------------------ */
/* Return true if 'x' has the same number of 3's and 4's in its base‑6
   representation.  Only integer operations are used. */
static bool has_equal34(seq_t x)
{
    int c3 = 0, c4 = 0;
    while (x > 0) {
        seq_t rem = x % (seq_t)6;
        if (rem == 3) {
            ++c3;
        } else if (rem == 4) {
            ++c4;
        }
        x = x / (seq_t)6;
    }
    return c3 == c4;
}

/* ------------------------------------------------------------------ */
/* Grow the cache array to at least 'need' elements. */
static void ensure_cache_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache     = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Extend the cache until it contains at least 'n' terms. */
static void extend_cache(idx_t n)
{
    while (cache_size < n) {
        /* Find the next number that satisfies the property. */
        while (!has_equal34(next_candidate))
            ++next_candidate;

        /* Allocate more space if needed. */
        ensure_cache_capacity(cache_size + 1);

        /* Store the found term and advance to the next candidate. */
        cache[cache_size++] = next_candidate;
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Exported sequence function. */
seq_t a(idx_t n)
{
    /* We expect n >= 1 (OEIS offset).  Behaviour for n == 0 is
       undefined but returning 0 keeps the function safe. */
    if (n == 0)
        return (seq_t)0;

    if (n > cache_size)
        extend_cache(n);

    return cache[n - 1];
}
