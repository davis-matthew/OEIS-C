/*
 * A131885  a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) for n >= 4 starting with
 *          a(0) = 1, a(1) = 2, a(2) = 4, and a(3) = 6.
 * https://oeis.org/A131885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) (n ≥ 4) a(0) = 1, a(1) =
 *            2, a(2) = 4, a(3) = 6
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_cache   = NULL;     /* cached values of the sequence   */
static size_t a_cache_sz = 0;       /* current size of the cache      */
static idx_t   a_last    = 0;       /* largest n that has been computed */
static bool    a_init    = false;   /* has the cache been initialised? */

/* Ensure the cache can hold indices up to n.  The cache is grown
   only when n is larger than the current size.  Memory allocation
   is done with malloc/realloc so that the code works in both C
   and C++.  */
static void
a_ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* we need indices 0..n inclusive */
    if (a_cache == NULL) {                  /* first allocation */
        size_t sz = needed < 4 ? 4 : needed;
        a_cache = (seq_t *)malloc(sz * sizeof(seq_t));
        a_cache_sz = sz;
    } else if (a_cache_sz <= needed) {      /* grow the cache */
        size_t new_sz = a_cache_sz * 2;
        if (new_sz <= needed)
            new_sz = needed + 1;
        a_cache = (seq_t *)realloc(a_cache, new_sz * sizeof(seq_t));
        a_cache_sz = new_sz;
    }
}

/* Return the nth term of the sequence.  The function can be called
   in any order; it uses memoisation to avoid recomputation. */
seq_t
a(idx_t n)
{
    /* initialise the cache and the first four terms */
    if (!a_init) {
        a_ensure_cache(n);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)2;
        a_cache[2] = (seq_t)4;
        a_cache[3] = (seq_t)6;
        a_last    = 3;
        a_init    = true;
    }

    /* if the requested index is larger than the last computed one,
       compute the missing terms using the recurrence. */
    if (n > a_last) {
        a_ensure_cache(n);
        for (idx_t i = a_last + 1; i <= n; ++i) {
            a_cache[i] = (seq_t)4 * a_cache[i - 1]
                        - (seq_t)6 * a_cache[i - 2]
                        + (seq_t)4 * a_cache[i - 3];
        }
        a_last = n;
    }

    /* a_cache[0]..a_cache[a_last] are now valid */
    return a_cache[(size_t)n];
}
