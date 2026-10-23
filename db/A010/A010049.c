/*
 * A010049  Second-order Fibonacci numbers.
 * https://oeis.org/A010049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + F(n-2) for n ≥ 2, where F(k) is the
 *            k‑th Fibonacci number. Initial values: a(0)=0, a(1)=1, a(2)=1.
 *            Recurrence from the OEIS entry
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 42, u64 n <= 87, u128 n <= 177,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>            /* malloc, realloc, abort */
#include "../utils/fib.h"       /* fib(idx_t) returning seq_t */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cached values of a(i) */
    static size_t cap   = 0;       /* allocated capacity of cache   */
    static size_t len   = 0;       /* number of elements stored     */

    /* first call: initialise the cache with a(0), a(1), a(2) */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;          /* a(0) */
        cache[1] = 1;          /* a(1) */
        cache[2] = 1;          /* a(2) */
        len = 3;               /* indices 0..2 are valid */
    }

    /* value already computed */
    if (n < (idx_t)len)
        return cache[n];

    /* extend cache up to index n */
    idx_t i_start = (idx_t)((len >= 2) ? len - 2 : 0);

    for (idx_t i = i_start; i <= n - 2; ++i) {
        /* grow cache if necessary */
        if (len >= cap) {
            size_t new_cap = cap * 2;
            if (new_cap == 0) new_cap = 4;   /* safety for first grow */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }

        /* recurrent step: a(i+2) = a(i+1) + a(i) + F(i) */
        seq_t Fi = fib(i);
        seq_t new_val = cache[i + 1] + cache[i] + Fi;

        cache[len] = new_val;
        ++len;
    }

    return cache[n];
}
