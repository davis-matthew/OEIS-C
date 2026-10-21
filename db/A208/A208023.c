/*
 * A208023  Number of n X 3 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A208023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A208023: Number of n X 3 0..1 arrays avoiding 0 0 0 and 0 0 1
   horizontally and 0 0 0 and 1 0 1 vertically.
   Recurrence: for n > 10
       a(n) = a(n-1) + a(n-2) + 8*a(n-3) + 4*a(n-4) + 3*a(n-5)
              - 5*a(n-6) - a(n-7) + a(n-9)
   Initial values: a(1)=6, a(2)=36, a(3)=102, a(4)=279, a(5)=741,
                   a(6)=1995, a(7)=5404, a(8)=14555, a(9)=39180,
                   a(10)=105688.
*/

#include <stddef.h>
#include <stdlib.h>

/* Computes a(n) for the OEIS sequence A208023. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Cached values.  cache[i] holds a(i+1).  */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed = 0;          /* highest index with value filled (1‑based) */

    /* Ensure the cache array is large enough. */
    if (cache_cap == 0 || cache_cap < n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { free(cache); /* abort on allocation failure */ return (seq_t)0; }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the known first 10 terms if we haven't yet. */
    if (computed < 10) {
        static const seq_t init[10] = {
            6ULL, 36ULL, 102ULL, 279ULL, 741ULL,
            1995ULL, 5404ULL, 14555ULL, 39180ULL, 105688ULL
        };
        idx_t start = computed + 1;
        if (start == 0) start = 1;
        for (idx_t i = start; i <= 10 && i <= n; ++i)
            cache[i - 1] = init[i - 1];
        computed = 10;
    }

    /* Grow the cache as needed, computing new values with the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 10) {			/* should not happen after init */
            /* Already handled above */
            continue;
        }
        /* recurrence uses indices i-1 .. i-9 (all >=1 for i >= 11) */
        seq_t val = cache[i-2]               /* a(i-1) */
                   + cache[i-3]               /* a(i-2) */
                   + 8 * cache[i-4]           /* 8*a(i-3) */
                   + 4 * cache[i-5]           /* 4*a(i-4) */
                   + 3 * cache[i-6];          /* 3*a(i-5) */
        val -= 5 * cache[i-7];                 /* -5*a(i-6) */
        val -= cache[i-8];                      /* -a(i-7) */
        val += cache[i-10];                     /* +a(i-9) */
        cache[i - 1] = val;
    }
    computed = n;
    return cache[n - 1];
}
