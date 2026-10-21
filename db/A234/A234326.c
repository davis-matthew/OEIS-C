/*
 * A234326  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 32*a(n-2) - 48*a(n-4)
 * Initial values: a(1)=80, a(2)=432, a(3)=2368, a(4)=13056
 * Offset: 1 (so a(1) is the first term)
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache: cache[1] ... cache[max_n] are defined */
    static seq_t *cache = NULL;      /* array of values, index 0 unused */
    static idx_t max_n = 0;          /* largest n that has been computed */

    /* Grow the cache if necessary */
    if (n > max_n) {
        size_t new_cap = (size_t)(n + 1);   /* need indices 0..n */
        seq_t *tmp;
#ifdef __cplusplus
        tmp = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
        tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) abort();                  /* allocation failed */
        cache = tmp;

        /* Compute missing values up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i == 1)        cache[i] = (seq_t)80;
            else if (i == 2)   cache[i] = (seq_t)432;
            else if (i == 3)   cache[i] = (seq_t)2368;
            else if (i == 4)   cache[i] = (seq_t)13056;
            else              cache[i] = (seq_t)32 * cache[i-2]
                                   - (seq_t)48 * cache[i-4];
        }
        max_n = n;
    }
    return cache[n];
}
