/*
 * A119515  Expansion of x*(x^3+2*x^2+3*x-1)/(x+1)^5.
 * https://oeis.org/A119515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 21, i32 n <= 29, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A119515: Expansion of x*(x^3+2*x^2+3*x-1)/(x+1)^5.
   Generating function gives the linear recurrence

      a(n) + 5*a(n-1) + 10*a(n-2) + 10*a(n-3) + 5*a(n-4) + a(n-5) = 0
        for n ≥ 5,

   with initial values
      a(0)=0,  a(1)=–1,  a(2)=8,  a(3)=–28,  a(4)=71.
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for already computed values */
    static seq_t *cache      = NULL;   /* pointer to cache array        */
    static idx_t cache_size  = 0;      /* current number of cached items */
    static idx_t last_computed = 4;    /* highest index that has been
                                          computed (initially 4 after init) */

    /* Initial values for n = 0..4 */
    static const seq_t init_vals[5] = {0, -1, 8, -28, 71};

    /* First call: allocate and initialise the cache */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* out‑of‑memory guard */
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = init_vals[i];
    }

    /* If the request is within the initial block, return it */
    if (n < 5)
        return cache[n];

    /* Grow the cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;           /* out‑of‑memory guard */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum =
            (seq_t)5 * cache[i - 1] +
            (seq_t)10 * cache[i - 2] +
            (seq_t)10 * cache[i - 3] +
            (seq_t)5 * cache[i - 4] +
            cache[i - 5];
        cache[i] = -sum;
    }
    last_computed = n;

    return cache[n];
}
