/*
 * A212743  Number of (w,x,y,z) with all terms in {0,...,n} and
 *          max{w,x,y,z}>2*min{w,x,y,z}.
 * https://oeis.org/A212743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): 
   a(n)=2*a(n-1)+2*a(n-2)-6*a(n-3)+6*a(n-5)-2*a(n-6)-2*a(n-7)+a(n-8)  for n≥8
   a(0)=0, a(1)=14, a(2)=64, a(3)=224, a(4)=528, a(5)=1134, a(6)=2064, a(7)=3584
   (Table values up to n=7 are provided explicitly.) */

#include <stddef.h>      /* size_t, NULL */
#include <stdint.h>      /* uint64_t */
#include <stdlib.h>      /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;   /* pointer to dynamic array */
    static size_t cache_len = 0;  /* number of valid entries filled (next free index) */

    /* Exact initial values for n=0..7 */
    static const seq_t init[8] = {
        (seq_t)0, (seq_t)14, (seq_t)64, (seq_t)224,
        (seq_t)528, (seq_t)1134, (seq_t)2064, (seq_t)3584
    };

    /* Direct return for the first eight terms */
    if (n < 8) {
        return init[(size_t)n];
    }

    /* First call: allocate space for the initial terms */
    if (cache == NULL) {
        size_t init_size = 8;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;             /* allocation failure */
        for (size_t i = 0; i < 8; ++i) {
            cache[i] = init[i];
        }
        cache_len = 8;                           /* indices 0..7 are filled */
    }

    /* Ensure cache is large enough to hold up to n */
    if ((size_t)n >= cache_len) {
        size_t old_len = cache_len;              /* current number of filled entries */
        size_t new_len = (size_t)n + 1;          /* need to store indices 0..n */
        if (new_len < 8) new_len = 8;            /* keep at least 8 slots */

        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* allocation failure */
        cache = tmp;

        /* Compute all missing terms from old_len up to n inclusive */
        for (size_t i = old_len; i <= (size_t)n; ++i) {
            /* recurrence */
            seq_t val = (seq_t)2 * cache[i-1]       /* 2*a(n-1) */
                     + (seq_t)2 * cache[i-2]       /* 2*a(n-2) */
                     - (seq_t)6 * cache[i-3]       /* -6*a(n-3) */
                     + (seq_t)6 * cache[i-5]       /* +6*a(n-5) */
                     - (seq_t)2 * cache[i-6]       /* -2*a(n-6) */
                     - (seq_t)2 * cache[i-7]       /* -2*a(n-7) */
                     + cache[i-8];                 /* +a(n-8) */
            cache[i] = val;
        }

        cache_len = new_len;                    /* now indices 0..n are valid */
    }

    return cache[(size_t)n];
}
