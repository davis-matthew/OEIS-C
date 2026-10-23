/*
 * A162830  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(0)=1, a(1)=28, a(2)=756,
 *            a(3)=20034, a(n) = 26*a(n-1) + 26*a(n-2) - 351*a(n-3) for n ≥ 4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)28;
    if (n == 2) return (seq_t)756;
    if (n == 3) return (seq_t)20034;

    /* Memoised dynamic array for all indices ≥ 4. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of allocated elements */

    /* Expand the cache if required. */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        cache = tmp;
        /* Initialise first four values if this is the first allocation. */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)28;
            cache[2] = (seq_t)756;
            cache[3] = (seq_t)20034;
        }
        /* Compute missing terms from the highest already–computed index
           (or 4 if less) up to 'n' using the recurrence. */
        idx_t start = cache_len;
        if (start < 4) start = 4;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = (seq_t)26 * cache[i-1] + (seq_t)26 * cache[i-2]
                       - (seq_t)351 * cache[i-3];
        }
        cache_len = new_len;
    }
    return cache[n];
}
