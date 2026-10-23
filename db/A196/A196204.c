/*
 * A196204  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,0,3,4,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-4) - a(n-5) for n > 5, with initial values
 *            a(1)=3, a(2)=5, a(3)=8, a(4)=11, a(5)=18
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy static cache for computed terms.  Index 0 is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity (not including index 0) */
    static idx_t computed = 0;    /* highest index for which cache[i] is valid */

    /* Handle the trivial case of n < 1 */
    if (n < 1) return (seq_t)0;

    /* First call: allocate the cache and store the initial five terms. */
    if (!cache) {
        /* start with a small capacity, grow as needed */
        cache_cap = 16;
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */

        cache[1] = (seq_t)3;  /* a(1) */
        cache[2] = (seq_t)5;  /* a(2) */
        cache[3] = (seq_t)8;  /* a(3) */
        cache[4] = (seq_t)11; /* a(4) */
        cache[5] = (seq_t)18; /* a(5) */
        computed = 5;
    }

    /* Grow the array if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;     /* double until large enough */
        seq_t *new_ptr = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!new_ptr) return (seq_t)0;         /* allocation failed */
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= computed) return cache[n];

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 5) {
            /* The first five terms are already known; this branch is
               here only for completeness if a very small n is
               requested after cache reallocation. */
            switch (i) {
                case 1: val = (seq_t)3; break;
                case 2: val = (seq_t)5; break;
                case 3: val = (seq_t)8; break;
                case 4: val = (seq_t)11; break;
                case 5: val = (seq_t)18; break;
                default: val = (seq_t)0; break; /* unreachable */
            }
        } else {
            /* a(n) = 2*a(n-1) - a(n-4) - a(n-5) */
            val = cache[i - 1] + cache[i - 1]; /* 2*a(i-1) */
            val -= cache[i - 4];
            val -= cache[i - 5];
        }
        cache[i] = val;
    }
    computed = n;
    return cache[n];
}
