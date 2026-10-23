/*
 * A041852  Numerators of continued fraction convergents to sqrt(448).
 * https://oeis.org/A041852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 254*a(n-2) - a(n-4) with initial values a(0)=21,
 *            a(1)=127, a(2)=5355, a(3)=32257
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* initialise cache with the first four terms */
    if (!cache)
    {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;   /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)21;
        cache[1] = (seq_t)127;
        cache[2] = (seq_t)5355;
        cache[3] = (seq_t)32257;
        cache_len = 4;
    }

    size_t idx = (size_t)n;

    if (idx < cache_len)
        return cache[idx];

    if (idx >= cache_cap)
    {
        size_t new_cap = cache_cap;
        while (new_cap <= idx)
            new_cap <<= 1;          /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;   /* realloc failed – unlikely in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    for (size_t i = cache_len; i <= idx; ++i)
    {
        if (i < 4)
            continue;   /* already initialised */
        cache[i] = cache[i - 2] * ((seq_t)254) - cache[i - 4];
    }
    cache_len = idx + 1;

    return cache[idx];
}
