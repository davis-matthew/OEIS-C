/*
 * A025255  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-3)*a(3) for n >= 4,
 *          with initial terms 1, 3, 1, 1.
 * https://oeis.org/A025255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(1)*a(n-1) + a(2)*a(n-2) + … + a(n-3)*a(3)
 *          for n ≥ 4, with a(1) = 1, a(2) = 3, a(3) = 1, a(4) = 1
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a static cache is used so that calls may be made in any order.
       The cache grows in blocks of size 1024; the first block is
       big enough for the initial values. */
    static seq_t *cache    = NULL;   /* 1‑based array of sequence terms */
    static idx_t   max     = 0;      /* highest index up to which we have computed */
    static size_t  cap     = 0;      /* current allocated capacity (indices 0..cap-1) */
    const size_t INCREMENT = 1024;   /* block size for growth */

    /* Allocate the cache lazily.  At the very first call we must
       reserve space for indices 1..4. */
    if (cache == NULL) {
        cap     = INCREMENT;
        cache   = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – should never happen */

        /* initialise the first four terms (indices 1..4) */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        max = 4;
    }

    /* Grow the cache if the requested index is beyond the current capacity */
    while (n >= cap) {
        size_t new_cap = cap + INCREMENT;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure – should never happen */
        cache = tmp;
        cap   = new_cap;
    }

    /* If the value has already been computed, return it immediately. */
    if (n <= max) {
        return cache[n];
    }

    /* Compute all missing terms from max+1 up to n. */
    for (idx_t i = max + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* For i >= 5 the sum runs over k=1..i-3.
           For i == 4 the sum is zero but i is already initialized. */
        for (idx_t k = 1; k <= i - 3; ++k) {
            sum += cache[k] * cache[i - k];
        }
        cache[i] = sum;
    }
    max = n;

    return cache[n];
}
