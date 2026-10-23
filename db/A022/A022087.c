/*
 * A022087  Fibonacci sequence beginning 0, 4.
 * https://oeis.org/A022087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*F(n) where F is Fibonacci, a(0)=0, a(1)=4, a(n) =
 *            a(n-1) + a(n-2) for n >= 2
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache across calls */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated capacity */
    static idx_t   max_n = 0;   /* highest n computed so far */

    /* Grow capacity if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 4;   /* start with small array */
        while ((size_t)n + 1 > new_cap)
            new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – let caller handle */
            return 0;
        }
        cache = tmp;
        cap   = new_cap;

        /* Initialise base cases on first allocation */
        if (max_n == 0) {
            if (cap > 0) cache[0] = (seq_t)0;
            if (cap > 1) cache[1] = (seq_t)4;
            max_n = 1;
        }
    }

    /* Return cached value if already computed */
    if (n <= max_n)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2];
    }
    max_n = n;

    return cache[n];
}
