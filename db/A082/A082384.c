/*
 * A082384  a(0)=1, a(n)=2^n+n^2-2*a(n-1).
 * https://oeis.org/A082384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(0)=1, a(1)=6, a(2)=5, a(3)=22, a(4)=13,
   a(n) = 3*a(n-1)+a(n-2)-11*a(n-3)+12*a(n-4)-4*a(n-5)  (n≥5) */

seq_t a(idx_t n)
{
    /* Static cache: values already computed up to cached_up_to */
    static seq_t *cache       = NULL;
    static idx_t cache_len    = 0;    /* number of allocated slots           */
    static idx_t cached_up_to = 0;    /* highest n computed so far           */

    /* Base case: initialise cache on first call */
    if (cache_len == 0) {
        /* Allocate space for the first 5 terms                    */
        cache_len = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) {
            return (seq_t)0;          /* allocation failure – unlikely in tests */
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)22;
        cache[4] = (seq_t)13;
        cached_up_to = 4;
    }
    /* If n already in cache, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* Extend cache so that it holds values up to n */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!new_cache) {
        /* allocation failure – unlikely in tests, return 0 */
        return (seq_t)0;
    }
    cache = new_cache;

    /* Compute missing terms iteratively using the 5‑term recurrence */
    for (idx_t k = cached_up_to + 1; k <= n; ++k) {
        /* The recurrence uses the previous five values.
           For k < 5 the initial values are already stored. */
        seq_t val;
        if (k < 5) {
            /* The first five terms are already set – this branch is only
               needed for safety if the caller changes cache_len externally. */
            continue;
        } else {
            val  = (seq_t)3 * cache[k - 1];
            val += cache[k - 2];
            val -= (seq_t)11 * cache[k - 3];
            val += (seq_t)12 * cache[k - 4];
            val -= (seq_t)4  * cache[k - 5];
        }
        cache[k] = val;
    }

    /* Update bookkeeping */
    cache_len    = new_len;
    cached_up_to = n;
    return cache[n];
}
