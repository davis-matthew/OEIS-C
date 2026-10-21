/*
 * A073551  Number of Fibonacci numbers F(k), k <= 10^n, which end in 3.
 * https://oeis.org/A073551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = (2*10^n - 20)/15 for n>1, recurrence a(n) = 11*a(n-1) -
 *            10*a(n-2) for n>=4, initial values: a(1)=2, a(2)=12, a(3)=132
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    static seq_t *cache = NULL;
    static idx_t cache_max = 0;

    /* Return cached value if already computed */
    if (cache && n <= cache_max) {
        return cache[n];
    }

    /* Allocate / extend cache to hold up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Handle allocation failure if needed */
        exit(1);
    }

    /* Copy previously computed values */
    if (cache) {
        for (size_t i = 1; i <= cache_max; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
    }

    cache = new_cache;
    cache_max = n;

    /* Set base values */
    if (n >= 1) cache[1] = (seq_t)2;
    if (n >= 2) cache[2] = (seq_t)12;
    if (n >= 3) cache[3] = (seq_t)132;

    /* Compute remaining values using recurrence */
    for (idx_t i = 4; i <= n; ++i) {
        cache[i] = (seq_t)11 * cache[i-1] - (seq_t)10 * cache[i-2];
    }

    return cache[n];
}
