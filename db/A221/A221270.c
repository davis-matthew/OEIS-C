/*
 * A221270  Number of n X 2 arrays of occupancy after each element moves to
 *          some king-move neighbor, with every occupancy equal to zero or
 *          two.
 * https://oeis.org/A221270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) + 4*a(n-3) - 2*a(n-5) Sequence
 *            A221270 Initial values (offset 1): a(1)=0, a(2)=6, a(3)=16,
 *            a(4)=52, a(5)=174 The recurrence applies for n>=6
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for i >= 1.
       We allocate at least (n+1) elements to store index n at cache[n].   */
    static seq_t *cache = NULL;
    static idx_t   cache_cap = 0;  /* allocated size (number of elements) */
    static idx_t   last_computed = 0; /* largest n for which cache[n] is valid */

    /* Ensure that we have room for indices 1..n.
       The array will always have cache[0] unused.                         */
    if (n + 1 > cache_cap) {
        /* Allocate new capacity, at least twice the old one or n+1.       */
        idx_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
        while (new_cap < n + 1) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;           /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize the first five terms if not already done.            */
    if (last_computed < 5) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)52;
        cache[5] = (seq_t)174;
        last_computed = 5;
    }

    /* If the requested term is already computed, return it.           */
    if (n <= last_computed) {
        return cache[n];
    }

    /* Compute terms from last_computed+1 up to n using the recurrence. */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        seq_t term = (seq_t)6 * cache[k - 1]
                   - (seq_t)10 * cache[k - 2]
                   + (seq_t)4  * cache[k - 3]
                   - (seq_t)2  * cache[k - 5];
        cache[k] = term;
    }
    last_computed = n;
    return cache[n];
}
