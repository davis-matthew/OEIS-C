/*
 * A165894  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*a(n-1)+19*a(n-2)+...+19*a(n-9)-190*a(n-10) for n ≥ 10,
 *            with initial values derived from the numerator: a(0)=1, a(1)=21,
 *            a(2)=420, a(3)=8400, a(4)=168000, a(5)=3360000, a(6)=67200000,
 *            a(7)=1344000000, a(8)=26880000000, a(9)=537600000000
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   numerator_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/numerator_term.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Grow cache to at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);          /* fatal, OOM */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    if (cache == NULL) grow_cache(16);

    if (target < cache_len) return cache[target];

    /* Compute terms up to 'target'. */
    for (size_t i = cache_len; i <= target; ++i)
    {
        seq_t sum = numerator_term(i);

        /* Add 19*a(i-1) ... 19*a(i-9).  Check bounds. */
        for (int k = 1; k <= 9 && i >= (size_t)k; ++k)
        {
            seq_t term = cache[i - k];
            for (int t = 0; t < 19; ++t) sum += term;
        }

        /* Subtract 190*a(i-10) if i >= 10. */
        if (i >= 10)
        {
            seq_t term = cache[i - 10];
            for (int t = 0; t < 190; ++t) sum -= term;
        }

        grow_cache(i + 1);
        cache[i] = sum;
    }

    return cache[target];
}
