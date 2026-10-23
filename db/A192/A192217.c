/*
 * A192217  a(1)=1; a(n) = n*lcm(n, a(n-1)) for n > 1.
 * https://oeis.org/A192217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = n*lcm(n, a(n-1)) for n > 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    static seq_t *cache = NULL;
    static idx_t computed = 0;
    static size_t capacity = 0;

    /* Ensure the cache is large enough for indices up to n. */
    if (cache == NULL || (size_t)n + 1 > capacity) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 2 * capacity + 2)
            new_cap = 2 * capacity + 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialize the first term if it hasn't been stored yet. */
    if (computed < 1) {
        cache[1] = (seq_t)1;
        computed = 1;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t ii   = (seq_t)i;
        seq_t g    = gcd(ii, prev);

        /* a(i) = i^2 * a(i-1) / g   with early division to reduce size. */
        seq_t t    = ii / g;           /* i/g is an integer. */
        seq_t val  = ii * t * prev;    /* (i * (i/g)) * a(i-1) */

        cache[i] = val;
    }

    computed = n;
    return cache[n];
}
