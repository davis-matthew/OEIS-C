/*
 * A041756  Numerators of continued fraction convergents to sqrt(398).
 * https://oeis.org/A041756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 798*a(n-4) - a(n-8) (n ≥ 8) Initial values: a(0)=19,
 *            a(1)=20, a(2)=379, a(3)=399, a(4)=15541, a(5)=15940,
 *            a(6)=302461, a(7)=318401
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* initial 8 terms */
    static const seq_t init[8] = {
        (seq_t)19,        (seq_t)20,        (seq_t)379,        (seq_t)399,
        (seq_t)15541,     (seq_t)15940,     (seq_t)302461,     (seq_t)318401
    };

    /* first call: allocate cache and copy initial terms */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* allocation failed – return 0 as a benign value */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
    }

    /* extend cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0; /* allocation failed – return 0 */
        cache = tmp;
        /* compute new entries */
        for (size_t i = cache_cap; i <= (size_t)n; ++i) {
            /* recurrence a(i) = 798*a(i-4) - a(i-8) */
            cache[i] = (seq_t)798 * cache[i - 4] - cache[i - 8];
        }
        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
