/*
 * A170199  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   p_n_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170199
   Recurrence: a(0) = 1.
   For n >= 1:
     a(n) = 44 * (a(n-1)+...+a(n-38))
            - 990 * a(n-39)
            + p(n)
   where p(0) = 1, p(n) = 2 for 1 <= n <= 38, p(39) = 1, and p(n)=0 for n > 39.
   This follows from the rational generating function
   (t^39 + 2 t^38 + ... + 2 t + 1) / (1 - 44 t - ... - 44 t^38 + 990 t^39).
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/p_n_value.h"

/* Helper: resize the cache if necessary */
static void ensure_cache_size(size_t new_len, seq_t **cache, size_t *cap)
{
    if (new_len <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
    while (new_cap < new_len)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                /* out of memory */
    *cache = tmp;
    *cap   = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* allocate or grow the cache */
    ensure_cache_size((size_t)n + 1, &cache, &cache_cap);

    /* already known? */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* grow if needed */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t sum = 0;
        idx_t lim = (i < 38) ? i : 38;
        for (idx_t j = 1; j <= lim; ++j)
            sum += cache[(size_t)(i - j)];

        seq_t val = sum * (seq_t)44;
        if (i >= 39)
            val -= cache[(size_t)(i - 39)] * (seq_t)990;
        val += p_n_value(i);

        cache[(size_t)i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
