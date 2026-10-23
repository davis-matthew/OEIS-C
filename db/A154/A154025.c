/*
 * A154025  a(n+2) = 64*a(n+1) - a(n), a(1)=0, a(2)=8.
 * https://oeis.org/A154025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+2) = 64*a(n+1) - a(n), with a(1)=0, a(2)=8 */
#include <stdlib.h>
#include <stdbool.h>

/* Cached sequence values; 1‑based index. */
static seq_t *cache = NULL;
/* Number of allocated slots (including slot 0 for 1‑based indexing). */
static size_t cache_cap = 0;
/* Highest index that has been computed so far. */
static idx_t cache_last = 1;
/* Flag indicating whether the base cases have been initialized. */
static bool cache_init_done = false;

seq_t a(idx_t n)
{
    /* Expand the cache if necessary. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;             /* slot 0 unused, slot 1..n used */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize base cases if this is the first use of the cache. */
    if (!cache_init_done) {
        cache[1] = (seq_t)0;
        if (cache_cap > 2) cache[2] = (seq_t)8;
        cache_init_done = true;
        cache_last = 1;
    }

    /* Ensure that a(2) is set if the caller asked for n ≥ 2. */
    if (n >= 2 && cache_last < 2) {
        cache[2] = (seq_t)8;
        cache_last = 2;
    }

    /* Compute forward until we have a(n). */
    while (cache_last < n) {
        idx_t i = cache_last;                       /* current last computed index, i ≥ 2 */
        seq_t ai   = cache[i];                      /* a(i) */
        seq_t aim1 = cache[i-1];                    /* a(i-1) */
        seq_t next = ai * (seq_t)64 - aim1;          /* a(i+1) = 64*a(i) - a(i-1) */
        cache[i+1] = next;
        ++cache_last;
    }

    return cache[n];
}
