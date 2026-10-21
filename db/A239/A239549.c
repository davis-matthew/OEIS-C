/*
 * A239549  Expansion of x/(1-8*x-12*x^2).
 * https://oeis.org/A239549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 12*a(n-2) for n > 1 Initial values: a(0) = 0,
 *            a(1) = 1 Implements the sequence A239549 with memoization
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>   /* for memcpy */

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;   /* pointer to array of seq_t */
    static idx_t computed = 0;      /* number of already computed entries */
    static idx_t capacity = 0;      /* current size of allocated array */

    /* Return cached value if already known */
    if (n < computed)
        return cache[n];

    /* Allocate or grow cache */
    if (cache == NULL) {                      /* first allocation */
        capacity = (n < 2 ? 2 : n + 1);
#ifdef __cplusplus
        cache = reinterpret_cast<seq_t*>(malloc(capacity * sizeof(seq_t)));
#else
        cache = malloc(capacity * sizeof(seq_t));
#endif
    } else if ((idx_t)(n + 1) > capacity) {    /* need more space */
        idx_t new_cap = capacity;
        while (new_cap < n + 1)
            new_cap <<= 1;
#ifdef __cplusplus
        seq_t *tmp = reinterpret_cast<seq_t*>(malloc(new_cap * sizeof(seq_t)));
        memcpy(tmp, cache, computed * sizeof(seq_t));
        free(cache);
        cache = tmp;
#else
        cache = realloc(cache, new_cap * sizeof(seq_t));
#endif
        capacity = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            cache[i] = ((seq_t)8) * cache[i - 1] + ((seq_t)12) * cache[i - 2];
        }
    }

    computed = n + 1;
    return cache[n];
}
