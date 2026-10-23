/*
 * A037506  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,0.
 * https://oeis.org/A037506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) * 5 + d[n], where d[1] = 1, d[2] = 2, d[3] = 0 and
 *            then repeat 1,2,0,1,2,0,... Base‑5 interpretation of the first n
 *            digits of that periodic sequence
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to memoize computed terms */
    static seq_t *cache = NULL;
    static idx_t  cached_max = 0;
    static size_t cap = 0;

    /* lazy allocation of cache */
    if (!cache) {
        cap = 16;                     /* initial capacity */
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out of memory, return 0 as error */
    }

    /* grow cache if necessary */
    if (n >= cap) {
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > cached_max) {
        static const seq_t pattern[3] = {1, 2, 0};
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            seq_t d = pattern[(idx_t)((i - 1) % 3)];
            if (i == 1) {
                cache[i] = d;                /* a(1) = 1 */
            } else {
                cache[i] = cache[i - 1] * (seq_t)5 + d;
            }
        }
        cached_max = n;
    }

    return cache[n];
}
