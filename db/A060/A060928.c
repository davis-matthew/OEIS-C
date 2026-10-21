/*
 * A060928  Expansion of 1/(1 - 5*x - 4*x^3).
 * https://oeis.org/A060928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 4*a(n-3) for n>=3; a(0)=1, a(1)=5, a(2)=25
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)25;
    }

    if (n < cache_size)
        return cache[n];

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp)
            return 0;   /* allocation failed; return 0 as a safe placeholder */

        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = (seq_t)5 * cache[i - 1] + (seq_t)4 * cache[i - 3];
        }

        cache_size = new_size;
    }

    return cache[n];
}
