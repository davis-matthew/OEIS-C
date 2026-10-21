/*
 * A075115  Binomial transform of A073145:
 *          a(n)=Sum(binomial(n,k)*A073145(k),(k=0,..,n)).
 * https://oeis.org/A075115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) (n ≥ 3) Initial values:
 *            a(0)=3, a(1)=2, a(2)=0 The function uses memoised recurrence to
 *            compute any n ≥ 0
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 51, u64 n <= 102,
 *            u128 n <= 204, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n) {
    /* Return stored terms for n < 3 directly. */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)0;

    /* Memoisation cache. */
    static seq_t *cache = NULL;      /* array of previously computed a(i) */
    static idx_t cache_size = 0;    /* number of elements allocated */

    /* Allocate cache if empty. */
    if (cache == NULL) {
        cache_size = 3;                     /* at least a(0), a(1), a(2) */
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* memory allocation failed */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)0;
    }

    /* Extend cache if necessary. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* memory allocation failed */
        cache = tmp;

        /* Compute new values using the recurrence. */
        for (idx_t i = cache_size; i < new_size; ++i) {
            seq_t t1 = (seq_t)2 * cache[i-1];
            seq_t t3 = (seq_t)2 * cache[i-2];
            seq_t t2 = (seq_t)2 * cache[i-3];
            cache[i] = t1 + t2 - t3;        /* = 2*a(i-1) - 2*a(i-2) + 2*a(i-3) */
        }
        cache_size = new_size;
    }

    return cache[n];
}
