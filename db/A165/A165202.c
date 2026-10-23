/*
 * A165202  Expansion of (1+x)/(1 - x + x^2)^2.
 * https://oeis.org/A165202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 126, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of (1+x)/(1 - x + x^2)^2.
   Sequence satisfies the linear recurrence
       a(0) =  1
       a(1) =  3
       a(2) =  3
       a(3) = -1
       a(n) =  2*a(n-1) - 3*a(n-2) + 2*a(n-3) - a(n-4)    for n ≥ 4   */

#include <stdlib.h>

/* static cache for computed terms                               */
static seq_t *a_cache      = NULL;  /* pointer to allocated array   */
static size_t a_cache_size = 0;     /* number of values computed   */
static size_t a_cache_cap  = 0;     /* current allocation capacity */

/* Grow the cache to at least 'needed' elements.               */
static void
ensure_a_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* need indices 0 … n   */
    if (a_cache_size >= needed)             /* already sufficient    */
        return;

    /* Allocate or enlarge the backing array.                 */
    if (a_cache_cap == 0)
        a_cache_cap = 64;                  /* start with 64 cells   */
    while (a_cache_cap < needed)
        a_cache_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(a_cache, a_cache_cap * sizeof(seq_t));
    if (!tmp)          /* allocation failed – give up                */
        return;
    a_cache = tmp;

    /* Initialise the first four terms if the cache is still empty. */
    if (a_cache_size == 0) {
        a_cache[0] = 1;
        a_cache[1] = 3;
        a_cache[2] = 3;
        a_cache[3] = -1;
        a_cache_size = 4;          /* we now have terms 0…3   */
    }

    /* If the required value is already in the cache, we are done. */
    if (a_cache_size >= needed)
        return;

    /* Compute new terms using the recurrence.  We start at the
       first index that has not yet been computed: a_cache_size.   */
    for (size_t i = a_cache_size; i < needed; ++i) {
        a_cache[i] =  2 * a_cache[i-1]
                    - 3 * a_cache[i-2]
                    + 2 * a_cache[i-3]
                    -   a_cache[i-4];
    }

    a_cache_size = needed;      /* all indices < needed are now valid   */
}

/* Return the n-th term of the sequence (OEIS A165202).          */
seq_t a(idx_t n)
{
    ensure_a_cache(n);
    return a_cache[(size_t)n];
}
