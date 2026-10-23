/*
 * A122057  a(n) = (n+1)! * (H(n+1) - H(2)), where H(n) are the harmonic
 *          numbers.
 * https://oeis.org/A122057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)! * (H(n+1) - H(2)) = (n+1)! * Sum_{k=3..n+1} 1/k
 *            Recurrence used: a(1)=0, a(2)=2; for n>=3 a(n) = (n+1) * a(n-1)
 *            + n! (using 0‑based index) This implementation memoises computed
 *            values to handle arbitrary order of calls efficiently
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of computed values */
    static seq_t *cache = NULL;       /* cache[i] holds a(i) */
    static idx_t max_n = 0;           /* largest n computed so far */
    static seq_t fact = 1;            /* factorial(max_n) = (max_n)!   */
    static size_t capacity = 0;       /* allocated size of cache       */

    /* Initialise cache with base cases a(1)=0, a(2)=2 */
    if (cache == NULL) {
        capacity = 3;                 /* indices 0..2 (0 unused)       */
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failed; return 0   */
        cache[0] = 0;                  /* unused                         */
        cache[1] = (seq_t)0;           /* a(1) */
        cache[2] = (seq_t)2;           /* a(2) */
        fact = (seq_t)2;               /* 2! */
        max_n = 2;
    }

    /* If the requested n is already computed, return it */
    if (n <= max_n) {
        return cache[n];
    }

    /* Extend cache up to n recomputing values using recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* factorial(i) = factorial(i-1) * i */
        fact *= (seq_t)i;

        /* a(i) = (i+1)*a(i-1) + i!  */
        seq_t val = (seq_t)(i + 1) * cache[i - 1] + fact;

        /* Grow cache if needed */
        if ((i + 1) > capacity) {
            capacity = i + 1;
            seq_t *tmp = (seq_t*)realloc(cache, capacity * sizeof(seq_t));
            if (tmp == NULL) {
                /* Memory allocation failed; return 0 as a safe value */
                return 0;
            }
            cache = tmp;
        }

        cache[i] = val;
    }

    max_n = n;
    return cache[n];
}
