/*
 * A218898  Number of 2 X n arrays of the minimum value of corresponding
 *          elements and their horizontal, diagonal or antidiagonal neighbors
 *          in a random 0..1 2 X n array.
 * https://oeis.org/A218898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-5); Initial values: a(1)=4,
 *            a(2)=6, a(3)=18, a(4)=34, a(5)=86
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 54, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We purposefully use a static cache that grows as needed.
   The cache is implemented in a way that works for both C11
   (where seq_t is an unsigned integer type) and C++17
   (where seq_t might be an arbitrary‑precision integer class).  */
#include <stdlib.h>
#ifdef __cplusplus
# include <new>
#endif

seq_t a(idx_t n)
{
    /* Cache of already computed terms.
       cache[i] holds a(i+1).  */
    static seq_t *cache = NULL;
    static idx_t   cache_size = 0;
    static size_t  cache_cap  = 0;   /* storage size in elem count */

    if (n == 0)                  /* below offset – undefined; return 0 */
        return 0;

    /* Return cached value if already known */
    if (n <= cache_size)
        return cache[n-1];

    /* Extend cache to hold up to n terms */
    idx_t needed = n;
    if (cache_cap < (size_t)needed) {
        size_t new_cap = (cache_cap ? cache_cap * 2 : 8);
        if (new_cap < (size_t)needed)
            new_cap = (size_t)needed;

#ifdef __cplusplus
        /* In C++ we use `new` so that objects are properly constructed. */
        seq_t *tmp = new seq_t[new_cap];
        if (cache) {
            for (size_t j = 0; j < cache_cap; ++j)
                tmp[j] = cache[j];
            delete[] cache;
        }
#else
        /* In C we use realloc, which works for unsigned integer seq_t. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp)                /* a very unlikely failure */
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to 'needed' */
    for (idx_t i = cache_size; i < needed; ++i) {
        if (i < 5) {   /* 0‑based index: 0→a1, 1→a2, …, 4→a5 */
            switch (i) {
                case 0: cache[i] = 4;   break;
                case 1: cache[i] = 6;   break;
                case 2: cache[i] = 18;  break;
                case 3: cache[i] = 34;  break;
                default: cache[i] = 86; break; /* i==4 */
            }
        } else {
            /* a(n) = a(n-1) + 3*a(n-2) - 2*a(n-5)  (n = i+1) */
            seq_t sum = cache[i-1] + 3 * cache[i-2];
            cache[i]  = sum - 2 * cache[i-5];
        }
    }

    cache_size = needed;
    return cache[n-1];
}
