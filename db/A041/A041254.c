/*
 * A041254  Numerators of continued fraction convergents to sqrt(139).
 * https://oeis.org/A041254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued‑fraction convergents to sqrt(139).
   Recurrence:
       a_0 = 11,
       a_{-1} = 1,
       a_n   = c_{n-1} * a_{n-1} + a_{n-2}   (n >= 1)
   where the continued‑fraction coefficients of sqrt(139) (after the first term)
   are periodic: [1,3,1,3,7,1,1,2,11,2,1,1,7,3,1,3,1,22].
   Thus for n>=1,
       a_n = coeff[(n-1) mod 18] * a_{n-1} + a_{n-2}. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Periodic coefficients after the initial term 11. */
    static const seq_t period[] = {
        1, 3, 1, 3, 7, 1, 1, 2,
        11, 2, 1, 1, 7, 3, 1, 3, 1, 22
    };
    const size_t period_len = sizeof(period) / sizeof(period[0]);

    if (n < cache_size)
        return cache[n];

    grow_cache((size_t)n + 1);

    seq_t p_prev2, p_prev1;
    if (cache_size == 0) {
        /* Initial base values: a_-1 = 1, a_-2 = 0 for the recurrence. */
        p_prev2 = 0;
        p_prev1 = 1;
    } else if (cache_size == 1) {
        /* Have only a_0 cached.  For n = 1 we need a_-1 = 1. */
        p_prev2 = 1;
        p_prev1 = cache[0];
    } else {
        p_prev2 = cache[cache_size - 2];
        p_prev1 = cache[cache_size - 1];
    }

    for (idx_t k = cache_size; k <= n; ++k) {
        seq_t coeff;
        if (k == 0) {
            coeff = (seq_t)11;
        } else {
            /* Use the periodic coefficients starting at k = 1. */
            coeff = period[(size_t)((k - 1) % period_len)];
        }
        seq_t p_k = coeff * p_prev1 + p_prev2;
        cache[cache_size] = p_k;
        cache_size++;
        p_prev2 = p_prev1;
        p_prev1 = p_k;
    }

    return cache[n];
}
