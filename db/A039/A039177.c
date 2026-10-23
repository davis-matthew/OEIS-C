/*
 * A039177  Numbers whose base-11 representation has the same number of 2's
 *          and 6's.
 * https://oeis.org/A039177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th non‑negative integer whose base‑11 representation
 *            contains an equal number of digits 2 and 6. (OEIS A039177)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cache of previously found terms.  cache[i] holds a(i+1).  */
static seq_t *cache = NULL;      /* pointer to cached values          */
static size_t cache_len = 0;     /* number of cached values          */
static size_t cache_cap = 0;     /* capacity of allocated array       */
static seq_t next_start = 0;     /* next integer to test             */

/* Test whether a number has the required equal counts of 2 and 6 in base 11. */
static inline bool ok(seq_t v)
{
    int c2 = 0, c6 = 0;
    seq_t x = v;
    while (x > 0) {
        int d = (int)(x % 11);
        if (d == 2) ++c2;
        else if (d == 6) ++c6;
        x /= 11;
    }
    return c2 == c6;
}

/* Grow the cache array when it becomes full.  */
static inline void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a new term to the cache. */
static inline void add_value(seq_t v)
{
    if (cache_len == cache_cap)
        grow_cache();
    cache[cache_len++] = v;
}

/* Compute the n-th term of the sequence.  n is 1‑based.  */
seq_t a(idx_t n)
{
    /* Initialise cache on first call.  */
    if (cache_len == 0) {
        add_value((seq_t)0);         /* a(1) = 0                */
        next_start = (seq_t)1;       /* start testing from 1    */
    }

    /* Find terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t val = next_start;
        if (ok(val))
            add_value(val);
        ++next_start;
    }

    return cache[n - 1];
}
