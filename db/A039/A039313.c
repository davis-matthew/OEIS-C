/*
 * A039313  Numbers whose base-7 representation has the same nonzero number of
 *          0's and 6's.
 * https://oeis.org/A039313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth number whose base-7 representation contains
   the same non‑zero number of 0s and 6s.  It builds the sequence on demand
   and caches the results for repeated calls. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* cached sequence values                */
static size_t cache_size = 0;    /* number of values already stored       */
static size_t cache_cap  = 0;    /* allocated capacity of cache array      */
static seq_t last_checked = (seq_t)0; /* last integer examined               */

/* Test whether a given number satisfies the condition. */
static bool matches(seq_t v)
{
    int count0 = 0, count6 = 0;
    while (v > (seq_t)0) {
        int digit = (int)(v % (seq_t)7);
        if (digit == 0)   count0++;
        else if (digit == 6) count6++;
        v /= (seq_t)7;
    }
    return (count0 == count6) && (count0 > 0);
}

/* Ensure the cache has capacity for at least 'needed' elements. */
static void ensure_cache(size_t needed)
{
    if (cache_cap >= needed) return;

    size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {  /* Allocation failed – abort with minimal error handling */
        exit(1);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Main entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* Offset starts at 1 – 0 not used */

    /* If we already know the value, return it. */
    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];

    /* Search for new terms until we have at least n. */
    seq_t candidate = last_checked + (seq_t)1;
    while (cache_size < (size_t)n) {
        if (matches(candidate)) {
            ensure_cache(cache_size + 1);
            cache[cache_size++] = candidate;
        }
        candidate += (seq_t)1;
    }
    last_checked = candidate - (seq_t)1;
    return cache[(size_t)n - 1];
}
