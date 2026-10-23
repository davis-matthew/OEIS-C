/*
 * A116780  Number of permutations of length n which avoid the patterns 2341,
 *          4123, 4132.
 * https://oeis.org/A116780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 19*a(n-2) + 27*a(n-3) - 19*a(n-4) + 8*a(n-5) -
 *            2*a(n-6) Initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=21,
 *            a(5)=74, a(6)=249
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build. */
static const seq_t init[7] = {0, 1, 2, 6, 21, 74, 249};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* current allocated size (number of slots) */
    static idx_t max_n_computed = 0;   /* highest n already computed */

    /* Grow the cache if necessary */
    if (cache_size < n + 1) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Compute any new terms */
    for (idx_t i = max_n_computed + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = init[i];
        } else {
            seq_t sum = 7 * cache[i - 1] + 27 * cache[i - 3] + 8 * cache[i - 5];
            seq_t diff = 19 * cache[i - 2] + 19 * cache[i - 4] + 2 * cache[i - 6];
            cache[i] = sum - diff;
        }
    }
    max_n_computed = n;

    return cache[n];
}
