/*
 * A212776  Half the number of 0..2 arrays of length n+2 with second
 *          differences nonzero.
 * https://oeis.org/A212776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence a(n) = a(n-1) + 2*a(n-2) + 3*a(n-3) + 2*a(n-4)
 *            Initial values (offset 1): a(1)=11, a(2)=27, a(3)=66, a(4)=162
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 47, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static memoisation table indexed by n (1‑based).
       cache[0] is unused and left as 0. */
    static seq_t *cache = NULL;          /* allocated array */
    static idx_t cache_capacity = 0;     /* size of allocated array   */
    static idx_t cache_last = 0;         /* highest n computed so far */

    /* Any requested n must be >=1; behaviour for n<1 is undefined. */

    if (n >= cache_capacity) {          /* grow table if needed */
        idx_t new_cap = cache_capacity > 0 ? cache_capacity * 2 : 5;
        while (new_cap <= n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure: return 0 */
        /* initialise newly allocated cells to 0 */
        for (idx_t i = cache_capacity; i < new_cap; ++i)
            cache[i] = 0;
        cache_capacity = new_cap;
    }

    if (cache_last == 0) {              /* first call: initialise base cases */
        /* Ensure capacity for at least 4 */
        if (cache_capacity < 5) {
            cache_capacity = 5;
            cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
            if (!cache) return 0;
        }
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)27;
        cache[3] = (seq_t)66;
        cache[4] = (seq_t)162;
        cache_last = 4;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* recurrence requires i-4 >= 1, which holds for i >= 5 */
        cache[i] = cache[i-1]
                 + (cache[i-2] << 1)
                 + (cache[i-3] * (seq_t)3)
                 + (cache[i-4] << 1);
    }
    cache_last = n > cache_last ? n : cache_last;

    return cache[n];
}
