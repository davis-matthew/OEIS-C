/*
 * A192313  Constant term of the reduction of n-th polynomial at A157751 by
 *          x^2->x+1.
 * https://oeis.org/A192313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192313: a(n) = 4*a(n-1) - a(n-2) - 6*a(n-3) - a(n-4)
   with a(1)=1, a(2)=2, a(3)=5, a(4)=13.  */

/* Needed for memory allocation and size_t */
#include <stdlib.h>

/* The function requested by the specification */
seq_t a(idx_t n)
{
    /* Static cache for arbitrary call order and memoisation */
    static seq_t *cache = NULL; /* array of computed terms */
    static idx_t cache_len = 0; /* number of terms stored (n values) */
    static idx_t cache_cap = 0; /* current capacity of the array */

    /* Safety: the sequence is defined for n>=1 */
    if (n == 0)
        return (seq_t)0; /* undefined, return 0 as a harmless sentinel */

    /* (Re)allocate/expand the cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap > 0 ? cache_cap : (idx_t)16;
        while (new_cap < n)
            new_cap <<= 1;                 /* double until large enough */

        /* realloc handles both first allocation and resizing */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – fall back to 0 */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first four terms on first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;  /* a(1) */
        cache[1] = (seq_t)2;  /* a(2) */
        cache[2] = (seq_t)5;  /* a(3) */
        cache[3] = (seq_t)13; /* a(4) */
        cache_len = 4;
    }

    /* If the requested term is already computed, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t val = ((seq_t)4) * cache[k - 2]      /* 4*a(k-1)   */
                    - cache[k - 3]                 /* -a(k-2)    */
                    - ((seq_t)6) * cache[k - 4]    /* -6*a(k-3)  */
                    - cache[k - 5];                /* -a(k-4)    */
        cache[k - 1] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
