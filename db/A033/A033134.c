/*
 * A033134  Base-7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,1,0.
 * https://oeis.org/A033134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A033134
   Recurrence: a(n) = 7*a(n-1) + a(n-3) - 7*a(n-4)
   Initial values: a(1)=1, a(2)=8, a(3)=56, a(4)=393
   Indexed from 1. */
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* Ensure that cache holds at least n terms (indices 1..n). */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n) return;

    size_t new_size = (size_t)n;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) return;                 /* allocation failed – leave cache unchanged */

    if (cache) {
        memcpy(new_cache, cache, (size_t)cache_size * sizeof(seq_t));
        free(cache);
    }

    /* initialise any newly allocated entries to 0 */
    for (idx_t i = cache_size; i < n; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_size = n;

    /* set the initial values (they may overwrite the 0 initialisation) */
    if (cache_size >= 1) cache[0] = (seq_t)1;
    if (cache_size >= 2) cache[1] = (seq_t)8;
    if (cache_size >= 3) cache[2] = (seq_t)56;
    if (cache_size >= 4) cache[3] = (seq_t)393;
}

/* Compute missing terms up to n by the recurrence.
   The array uses zero‑based indexing: cache[i] holds a(i+1). */
static void compute_up_to(idx_t n)
{
    /* All previously stored terms are already computed.
       Any new entries are still zero, which we can use as a sentinel. */
    for (idx_t i = 4; i < n; ++i) {
        if (cache[i] == (seq_t)0) {
            cache[i] =
                (seq_t)7 * cache[i - 1]           /*  a(i) = 7*a(i-1)   */
              +            cache[i - 3]          /*           + a(i-3) */
              - (seq_t)7 * cache[i - 4];        /*           - 7*a(i-4) */
        }
    }
}

/* External function implementing a(n). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    ensure_cache(n);
    compute_up_to(n);
    return cache[n - 1];
}
