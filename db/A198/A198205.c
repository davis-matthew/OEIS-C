/*
 * A198205  Number of 2nX2 0..3 arrays with values 0..3 introduced in row
 *          major order and each element unequal to exactly two horizontal and
 *          vertical neighbors.
 * https://oeis.org/A198205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 23*a(n-1) - 103*a(n-2) + 81*a(n-3) with initial values
 *            a(1)=4, a(2)=43, a(3)=658
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle requested index */
    if (n == 0) return 0;                /* undefined for offset 1, but safe */
    if (n < 4) {
        /* Direct lookup for the first three terms */
        static const seq_t init[4] = {0, 4, 43, 658};    /* init[0] unused */
        return init[n];
    }

    /* Static cache for already computed values */
    static seq_t *cache = NULL;     /* array of size (max_n + 1) */
    static size_t cap   = 0;        /* current capacity of cache */
    static size_t max_n = 0;        /* largest n that has been computed */

    /* Initialize the cache at first use */
    if (cap == 0) {
        /* Allocate space for at least n = 3 (a[1..3]) */
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure – return 0 */
        cache[1] = 4;
        cache[2] = 43;
        cache[3] = 658;
        max_n = 3;
    }

    /* Grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;  /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure – return 0 */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute new terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* Use the empirical recurrence */
        seq_t val = 23 * cache[i-1] + 81 * cache[i-3] - 103 * cache[i-2];
        cache[i] = val;
    }
    max_n = n;

    return cache[n];
}
