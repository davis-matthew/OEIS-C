/*
 * A177467  Expansion of g.f. (1+2*x+3*x^2)/(1-3*x-14*x^2+15*x^3+7*x^4).
 * https://oeis.org/A177467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 14*a(n-2) - 15*a(n-3) - 7*a(n-4) for n > 4,
 *            a(0)=1, a(1)=5, a(2)=32, a(3)=151, a(4)=819
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
    if (!new_cache)
        exit(1);

    /* Copy existing values */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* Initialize new entries */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0)
            new_cache[0] = 1;
        else if (i == 1)
            new_cache[1] = 5;
        else if (i == 2)
            new_cache[2] = 32;
        else if (i == 3)
            new_cache[3] = 151;
        else if (i == 4)
            new_cache[4] = 819;
        else
            new_cache[i] = 3 * new_cache[i-1] + 14 * new_cache[i-2]
                           - 15 * new_cache[i-3] - 7 * new_cache[i-4];
    }

    free(cache);
    cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
