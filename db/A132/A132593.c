/*
 * A132593  Nonnegative integer solutions X to the equation: X(X + 1) - 10*Y^2
 *          = 0.
 * https://oeis.org/A132593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 9; a(n) = 38*a(n-1) - a(n-2) + 18 for n >= 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 0;

    if (n == 0)
        return (seq_t)0;

    if (cache == NULL) {                     /* initial cache allocation */
        cache_size = 2;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)9;
        last_computed = 1;
    }

    if (n <= last_computed)
        return cache[n];

    if (n >= cache_size) {                   /* grow cache if needed */
        cache_size = n + 1;
        cache = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
    }

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t prev      = cache[i - 1];
        seq_t prevprev  = cache[i - 2];
        seq_t val = (seq_t)38 * prev;
        val = val - prevprev;
        val = val + (seq_t)18;
        cache[i] = val;
    }

    last_computed = n;
    return cache[n];
}
