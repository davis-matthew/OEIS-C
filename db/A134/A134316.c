/*
 * A134316  a(n) = index of first derangement of 1..n (n>=2).
 * https://oeis.org/A134316
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = a(n-2) + (n-1)!; Initial values: a(0)=1, a(1)=2.
 *            Computation uses memoisation: values are stored for subsequent
 *            calls in any order
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;       /* array of sequence values */
    static idx_t cache_size = 0;      /* current allocated size */
    static idx_t max_computed = 0;    /* highest index whose value is known */
    static seq_t fact = 1;            /* holds (max_computed-1)! after each step */

    /* handle first call: allocate minimum storage and initialise base cases */
    if (cache_size == 0) {
        cache_size = n + 1;           /* allocate enough for requested n */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failed; return 0 as sentinel */

        /* set known base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        if (n >= 2) {
            cache[2] = (seq_t)2;      /* a(2) = 2 */
            max_computed = 2;
        } else {
            max_computed = 0;
        }
        fact = (seq_t)1;            /* (1)! = 1, the factorial needed for n=2 */
    }

    /* ensure the cache is large enough for requested n */
    if ((idx_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (new_cache == NULL) return 0; /* allocation failed */
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* compute missing values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* update factorial to (i-1)! */
        fact = fact * (seq_t)(i - 1);
        /* recurrence: a(i) = a(i-2) + (i-1)! */
        cache[i] = cache[i - 2] + fact;
    }
    max_computed = n;

    return cache[n];
}
