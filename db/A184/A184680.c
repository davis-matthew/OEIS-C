/*
 * A184680  Number of (n+1) X 4 binary arrays with every 2 X 2 subblock
 *          singular.
 * https://oeis.org/A184680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 2*a(n-2) - 40*a(n-3) + 24*a(n-4) + 16*a(n-5)
 *            with a(1)=76, a(2)=544, a(3)=3360, a(4)=21968, a(5)=140816
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices – the offset is 1. */
    if (n == 0) return (seq_t)0;

    /* Initial values for a(1) … a(5). */
    static const seq_t init[5] = {
        (seq_t)76,
        (seq_t)544,
        (seq_t)3360,
        (seq_t)21968,
        (seq_t)140816
    };

    if (n <= 5) return init[n - 1];

    /* Static cache: cache[i] holds a(i) for i ≥ 1.  cache[0] is unused. */
    static seq_t *cache = NULL;     /* pointer to cache array */
    static size_t cap   = 0;        /* allocated capacity (in elements) */
    static idx_t max    = 0;        /* largest n has already been filled */

    /* Allocate or grow the cache if needed. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 16; /* double or start with 16 */
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed – give up */
        cache = tmp;
        cap   = new_cap;
    }

    /* Fill in the first five entries once.  cache[0] is left as 0. */
    if (max < 5) {
        memcpy(&cache[1], init, 5 * sizeof(seq_t));
        max = 5;
    }

    /* Compute new terms only when required. */
    for (idx_t i = max + 1; i <= n; ++i) {
        seq_t val =  cache[i - 1] * (seq_t)7
                  + cache[i - 2] * (seq_t)2
                  - cache[i - 3] * (seq_t)40
                  + cache[i - 4] * (seq_t)24
                  + cache[i - 5] * (seq_t)16;
        cache[i] = val;
    }

    max = n;
    return cache[n];
}
