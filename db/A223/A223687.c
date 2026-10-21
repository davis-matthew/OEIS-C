/*
 * A223687  Petersen graph (8,2) coloring a rectangular array: number of n X 3
 *          0..15 arrays where 0..15 label nodes of a graph with edges 0,1 0,8
 *          8,14 8,10 1,2 1,9 9,15 9,11 2,3 2,10 10,12 3,4 3,11 11,13 4,5 4,12
 *          12,14 5,6 5,13 13,15 6,7 6,14 7,0 7,15 and every array movement to
 *          a horizontal or antidiagonal...
 * https://oeis.org/A223687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 144, a(2) = 2304, ** Recurrence: a(n) = 24*a(n-1) -
 *            127*a(n-2) (n ≥ 3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;   /* array of computed terms */
static size_t cap   = 0;      /* current capacity of cache */
static idx_t  last  = 0;      /* highest index with a computed value */


/* Ensure that cache can hold indices 0 … needed (inclusive). */
static void ensure_cache(size_t needed)
{
    if (cap < needed) {
        size_t new_cap = (cap ? cap << 1 : 16);
        while (new_cap < needed)
            new_cap <<= 1;
        cache = seq_realloc(cache, cap, new_cap);  /* new_cap >= needed */
        cap = new_cap;
    }
}


seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined – return 0 for safety */
        return (seq_t)0;

    /* Lazy initialization on first use. */
    if (last == 0) {
        ensure_cache(3);            /* need indices 0..2 */
        cache[1] = (seq_t)144;
        cache[2] = (seq_t)2304;
        last = 2;
    }

    if (n <= last)
        return cache[n];

    ensure_cache(n + 1);          /* safe indexing up to n */

    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)24  * cache[i - 1];
        seq_t term2 = (seq_t)127 * cache[i - 2];
        cache[i] = term1 - term2;
    }

    last = n;
    return cache[n];
}
