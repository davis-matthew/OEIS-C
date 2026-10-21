/*
 * A073469  Expansion of x/B(x) where B(x) is the g.f. for A002487.
 * https://oeis.org/A073469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 44, i16 n <= 215, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A073469
 *  Recurrence: a(0) = 1
 *             a(n) = -a(n-1) - a(n-2) + (n even ? a(n/2) : 0)   for n ≥ 1
 *
 *  The implementation keeps a dynamic cache of all previously computed
 *  values so that each call to a(n) runs in O(n) time and O(n) memory.
 */
#include <stdlib.h>
#include <stdbool.h>

/* ----------  static cache state  ---------- */
static seq_t *cache = NULL;     /* array holding a(0) … a(size-1)          */
static size_t cache_cap = 0;    /* allocated capacity (bytes)             */
static size_t cache_size = 0;   /* number of terms actually computed     */

/* Grow the cache so that it can hold at least 'need' elements.
 * New entries are zero‑initialised.  The function preserves already
 * computed values.
 */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* unrecoverable memory error          */

    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;                  /* initialise newly added entries      */

    cache = tmp;
    cache_cap = new_cap;
}

/* Compute all terms a(i) for i from cache_size up to n (inclusive). */
static void compute_upto(idx_t n)
{
    /* Nothing to do if n is already covered. */
    if ((size_t)n < cache_size)
        return;

    /* Resize array to hold index n. */
    ensure_capacity((size_t)n + 1);

    /* Initialise the first term if this is the first call. */
    if (cache_size == 0) {
        cache[0] = 1;
        cache_size = 1;
    }

    /* Compute remaining terms. */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t val = 0;

        if (i >= 1)
            val -= cache[i - 1];
        if (i >= 2)
            val -= cache[i - 2];

        if ((i & 1) == 0)           /* even i → add a(i/2)   */
            val += cache[i >> 1];

        cache[i] = val;
        ++cache_size;
    }
}

/* ----------  public entry point  ---------- */
seq_t a(idx_t n)
{
    compute_upto(n);
    return cache[(size_t)n];
}
