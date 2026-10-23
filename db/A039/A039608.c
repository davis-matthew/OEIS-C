/*
 * A039608  Numbers whose base-12 representation has the same nonzero number
 *          of 1's and 6's.
 * https://oeis.org/A039608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039608 – numbers whose base‑12 representation has the
   same non‑zero number of 1’s and 6’s.  The function returns the
   n‑th such number (1‑based index).  It builds a cache of found
   values so that subsequent calls are O(1) for already computed
   indices. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"   /* for alloc_seq_array */

static seq_t *cache = NULL;          /* array of found values          */
static size_t cache_size = 0;        /* number of values currently stored */
static size_t cache_cap   = 0;        /* allocated capacity                 */
static seq_t last_candidate = 0;     /* last candidate examined           */

static bool good_value(seq_t x)
{
    unsigned count1 = 0, count6 = 0;
    while (x > 0)
    {
        unsigned digit = (unsigned)(x % 12);
        if (digit == 1) ++count1;
        else if (digit == 6) ++count6;
        x /= 12;
    }
    return (count1 == count6) && (count1 > 0);
}

static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_p = alloc_seq_array(new_cap);
    if (!new_p)
        return;  /* allocation failed – keep existing cache */
    if (cache)
    {
        memcpy(new_p, cache, cache_size * sizeof *cache);
        /* Do not free the old buffer: it may contain objects that
           require explicit deletion (e.g. arbitrary‑precision
           sequences).  A small memory leak is acceptable for the
           expected range of n. */
    }
    cache = new_p;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* invalid index – return 0 */

    /* Return cached value if it exists. */
    if (n <= cache_size && cache)
        return cache[n - 1];

    /* Ensure the cache is large enough. */
    ensure_capacity((size_t)n);

    /* Search for the next required numbers. */
    seq_t candidate = last_candidate;
    for (size_t count = cache_size; count < (size_t)n; ++count)
    {
        do
        {
            ++candidate;
        } while (!good_value(candidate));
        cache[count] = candidate;
    }

    last_candidate = candidate;
    cache_size = n;
    return cache[n - 1];
}
