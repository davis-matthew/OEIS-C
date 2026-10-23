/*
 * A122582  a(n) = a(n - 1) - 2*a(n - 2) + a(n - 3) - 2*a(n - 4) + a(n - 5).
 * https://oeis.org/A122582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 2*a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) with
 *            a(1)=a(2)=a(3)=a(4)=a(5)=1
 * Range:     i8 n <= 21, i16 n <= 42, i32 n <= 84, i64 n <= 169,
 *            i128 n <= 338, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n < 1, but callers only use n >= 1 */
    if (n <= 5) return (seq_t)1;         /* initial values */

    static seq_t *cache = NULL;
    static idx_t computed_up_to = 0;     /* largest index already computed in cache */
    static size_t cache_cap = 0;         /* allocated size of cache (indices 0..cache_cap-1) */

    /* Ensure cache is big enough for index n */
    if (cache_cap < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        cache = tmp;
        cache_cap = (size_t)(n + 1);
    }

    /* Initialize first five terms once */
    if (computed_up_to < 5) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)1;
        computed_up_to = 5;
    }

    /* Compute any missing terms up to n */
    if (computed_up_to < n) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            cache[i] = cache[i-1]
                       - ((seq_t)2) * cache[i-2]
                       + cache[i-3]
                       - ((seq_t)2) * cache[i-4]
                       + cache[i-5];
        }
        computed_up_to = n;
    }

    return cache[n];
}
