/*
 * A154148  Numbers k such that 21 plus the k-th triangular number is a
 *          perfect square.
 * https://oeis.org/A154148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A154148: Numbers k such that 21 + k*(k+1)/2 is a perfect square.
   The sequence satisfies the 5‑term recurrence
       a(n) = a(n-1) + 6*a(n-2) - 6*a(n-3) - a(n-4) + a(n-5)
   with initial values
       a(1)=5, a(2)=7, a(3)=40, a(4)=50, a(5)=237
   This implementation caches previously computed terms so that calls
   are independent of ordering and run in O(n) time for the largest
   requested n.
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently stored (>=0) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Initial fixed terms (index 1 ... 5) */
    static const seq_t init[5] = { (seq_t)5, (seq_t)7, (seq_t)40, (seq_t)50, (seq_t)237 };

    /* For n <= 5 we can return immediately without allocating */
    if (n <= 5) return init[n-1];

    /* Ensure cache holds at least the first five terms */
    if (cache_len < 5) {
        cache_cap = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 5; ++i) cache[i] = init[i];
        cache_len = 5;
    }

    /* Grow cache if needed */
    if (n > cache_cap) {
        cache_cap = n;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + 6*a(i-2) - 6*a(i-3) - a(i-4) + a(i-5) */
        seq_t val = cache[i-2]            /* a(i-1) */
                 + 6 * cache[i-3]         /* +6*a(i-2) */
                 - 6 * cache[i-4]         /* -6*a(i-3) */
                 -   cache[i-5]           /* -a(i-4) */
                 +   cache[i-6];          /* +a(i-5) */
        cache[i-1] = val;
    }
    cache_len = n;
    return cache[n-1];
}
