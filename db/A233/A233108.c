/*
 * A233108  Number of n X 3 0..5 arrays with no element x(i,j) adjacent to
 *          value 5-x(i,j) horizontally or vertically, top left element zero,
 *          and 1 appearing before 2 3 and 4, and 2 appearing before 3 in row
 *          major order.
 * https://oeis.org/A233108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 111*a(n-1) - 2128*a(n-2) + 10532*a(n-3) - 17559*a(n-4) +
 *            9045*a(n-5) with initial values: a(1)=6, a(2)=313, a(3)=24912,
 *            a(4)=2154065, a(5)=189286871
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation memoises all computed terms so that calls
   can arrive in any order and are O(n) in total.  All arithmetic
   is done with the native operators; for an arbitrary‑precision
   seq_t the compiler will use the appropriate overloads.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation table: cache[i] holds a(i) for i ≥ 1 */
    static seq_t *cache = NULL;
    static idx_t cache_size   = 0; /* number of allocated slots */
    static idx_t last_stored  = 0; /* largest n for which cache[n] is valid */

    /* First call: initialise cache with the first five terms. */
    if (cache == NULL) {
        /* allocate space for indices 0..5 (index 0 unused) */
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) {          /* allocation failed – give up */
            return (seq_t)0;
        }
        /* fill with zeros for safety */
        for (idx_t i = 0; i < cache_size; ++i) cache[i] = (seq_t)0;
        /* known initial values */
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)313;
        cache[3] = (seq_t)24912;
        cache[4] = (seq_t)2154065;
        cache[5] = (seq_t)189286871;
        last_stored = 5;
    }

    /* Return immediately if already computed. */
    if (n <= last_stored) {
        return cache[n];
    }

    /* Ensure the cache is large enough.  Growth is by doubling to keep
       amortised linear behaviour. */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) { /* realloc failed – give up */
            return (seq_t)0;
        }
        cache = tmp;
        /* initialise new slots to zero */
        for (idx_t i = cache_size; i < new_size; ++i) cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* Compute terms from last_stored+1 up to n using the recurrence. */
    for (idx_t k = last_stored + 1; k <= n; ++k) {
        /* recurrence valid only for k >= 6 – for k <= 5 the loop is
           reached only if initial allocation was smaller, which we know
           cannot happen because we pre‑filled indices 1..5 above. */
        seq_t term =
            (seq_t)111 * cache[k-1] -
            (seq_t)2128 * cache[k-2] +
            (seq_t)10532 * cache[k-3] -
            (seq_t)17559 * cache[k-4] +
            (seq_t)9045 * cache[k-5];
        cache[k] = term;
    }

    last_stored = n;
    return cache[n];
}
