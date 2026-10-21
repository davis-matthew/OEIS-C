/*
 * A221323  Number of n X 3 arrays of occupancy after each element stays put
 *          or moves to some horizontal or antidiagonal neighbor, with no
 *          occupancy greater than 2.
 * https://oeis.org/A221323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 23*a(n-1) - 110*a(n-2) + 111*a(n-3) - 6*a(n-4) -
 *            8*a(n-5), with a(1)=7, a(2)=119, a(3)=2008, a(4)=33873,
 *            a(5)=571358
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function is memoised and works for any build where
   seq_t is an integer type that may be arbitrarily large and
   idx_t is an unsigned integer type.  The implementation
   uses platform‑specific dynamic memory allocation, ensuring
   it compiles both as C11 and as C++17.  */

#ifndef __cplusplus
# include <stdlib.h>    /* malloc, realloc, free */
# include <stddef.h>    /* size_t */
#else
# include <new>         /* operator new[] / delete[] */
# include <cstddef>     /* size_t */
#endif

static seq_t* allocate_array(idx_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc((size_t)n * sizeof(seq_t));
#endif
}

static void free_array(seq_t* p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cached = 0;

    if (n == 0) {                /* undefined for n=0, but avoid crash */
        return (seq_t)0;
    }

    /* Return cached value if available */
    if (n <= cached) {
        return cache[n - 1];
    }

    /* Allocate a new array of the required size and copy existing data */
    seq_t *new_cache = allocate_array(n);
    if (!new_cache) { /* allocation failed */
        exit(1);
    }
    for (idx_t i = 0; i < cached; ++i) {
        new_cache[i] = cache[i];
    }
    free_array(cache);
    cache = new_cache;

    /* Base values for n = 1 … 5 */
    const seq_t base[5] = { (seq_t)7,
                            (seq_t)119,
                            (seq_t)2008,
                            (seq_t)33873,
                            (seq_t)571358 };

    /* Compute the missing terms up to n */
    for (idx_t i = cached; i < n; ++i) {
        if (i < 5) {
            cache[i] = base[i];
        } else {
            cache[i] = (seq_t)23 * cache[i - 1]
                     - (seq_t)110 * cache[i - 2]
                     + (seq_t)111 * cache[i - 3]
                     - (seq_t)6   * cache[i - 4]
                     - (seq_t)8   * cache[i - 5];
        }
    }
    cached = n;

    return cache[n - 1];
}
