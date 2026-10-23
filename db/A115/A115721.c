/*
 * A115721  Table of Durfee square of partitions in Abramowitz and Stegun
 *          order.
 * https://oeis.org/A115721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Durfee square of the n‑th partition in Abramowitz &
 *            Stegun order
 * Range:     u8 n <= 2713, u16 n <= 2713, u32 n <= 2713, u64 n <= 2713,
 *            u128 n <= 2713, big any n
 * Helpers:   durfee
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/durfee.h"

/* ---------- Static cache for the sequence values ---------- */
static seq_t *seq_cache = NULL;   /* Dynamic array of computed a(n) values   */
static size_t   cache_cap  = 0;   /* Current capacity of seq_cache         */
static size_t   cache_len  = 0;   /* Number of values stored in seq_cache  */
static int      last_m     = 0;   /* Highest integer m that has been processed */

/* Ensure seq_cache has enough space for another element */
static void ensure_cache_capacity(void)
{
    if (cache_len < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* Out of memory */
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Append a new value to the cache */
static void append_value(seq_t v)
{
    ensure_cache_capacity();
    seq_cache[cache_len++] = v;
}

/*
 * Recursively generate all partitions of 'rem' into parts not exceeding
 * 'max_allowed', storing each partition in 'parts' and computing its Durfee
 * square when a partition is completed (rem == 0).
 *
 * The recursion enumerates partitions in ascending order of the largest part.
 */
static void gen_parts(int rem, int max_allowed, int *parts, int depth)
{
    if (rem == 0) {
        append_value(durfee(parts, depth));
        return;
    }
    int upper = (max_allowed < rem) ? max_allowed : rem;
    for (int first = 1; first <= upper; ++first) {
        parts[depth] = first;
        gen_parts(rem - first, first, parts, depth + 1);
    }
}

/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* First time: initialise the cache with a(0) = 0 */
    if (!seq_cache) {
        cache_cap = 64;
        seq_cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!seq_cache) abort();
        seq_cache[0] = 0;      /* a(0) */
        cache_len = 1;
        last_m = 0;
    }

    /* Return cached value if already computed */
    if ((size_t)n < cache_len)
        return seq_cache[n];

    /* Buffer for building partitions; size sufficient for any m encountered */
    int parts[512];

    /* Generate rows m = 1,2,3,… until the cache covers index n */
    while ((size_t)n >= cache_len) {
        int m = last_m + 1;                  /* Next integer to process */
        gen_parts(m, m, parts, 0);           /* Generate all partitions of m */
        last_m = m;
    }

    return seq_cache[n];
}
