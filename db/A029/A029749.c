/*
 * A029749  Numbers of the form 2^k times 1, 5 or 7.
 * https://oeis.org/A029749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 91, u64 n <= 187,
 *            u128 n <= 379, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029749: Numbers of the form 2^k × (1,5,7).
   Recurrence: a(n) = 2 * a(n-3) for n > 4
   Initial terms: a(0)=1, a(1)=2, a(2)=4, a(3)=5, a(4)=7
*/
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

static seq_t get_a(idx_t n)
{
    if (n < cache_cap && cache[n] != 0)
        return cache[n];

    seq_t val;
    switch (n) {
        case 0: val = 1; break;
        case 1: val = 2; break;
        case 2: val = 4; break;
        case 3: val = 5; break;
        case 4: val = 7; break;
        default: val = 2 * get_a(n - 3);
    }

    if ((size_t)n >= cache_cap) {
        size_t old_cap = cache_cap;
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return val;               /* allocation failed – return the value */
        for (size_t i = old_cap; i < cache_cap; ++i)
            cache[i] = 0;
    }

    cache[n] = val;
    return val;
}

seq_t a(idx_t n)
{
    return get_a(n);
}
