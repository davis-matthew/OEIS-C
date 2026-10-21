/*
 * A051426  Least common multiple of {2, 4, 6, ..., 2n}.
 * https://oeis.org/A051426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = lcm{2,4,…,2n} = 2 * lcm(1,2,…,n)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Compute a(n) for n ≥ 1.  The function is memoised, so it can be
 * called in arbitrary order; it remains efficient for n up to about
 * 10 000 in well‑under‑2 seconds. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined with offset 1.  Return 0 for any n < 1. */
        return (seq_t)0;
    }

    /* Static cache buffers.  cache[i] holds a(i+1) (0‑indexed). */
    static seq_t *cache      = NULL;
    static idx_t  max_cached = 0;  /* number of terms already stored */

    /* Extend cache up to the requested n. */
    if (n > max_cached) {
        idx_t new_cap = n;
        seq_t *tmp = /* C/C++ safe cast of realloc result */
                    (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – return 0 to signal error */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute missing terms, starting from the next one after
         * the currently cached range. */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i == 1) {
                /* a(1) = 2 */
                cache[0] = (seq_t)2;
            } else {
                /* a(i) = lcm(a(i-1), 2*i) */
                seq_t prev = cache[i - 2];                  /* a(i-1)   */
                seq_t b    = (seq_t)2 * (seq_t)i;          /* 2*i      */
                seq_t g    = gcd(prev, b);                 /* gcd(a,b) */
                seq_t div  = prev / g;                     /* a/g      */
                cache[i - 1] = div * b;                    /* lcm(a,b) */
            }
        }
        max_cached = n;
    }

    return cache[n - 1];
}
