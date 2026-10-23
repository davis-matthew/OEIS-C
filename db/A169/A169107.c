/*
 * A169107  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 44 * (a(n-1)+a(n-2)+...+a(n-24)) - 990 * a(n-25) + rhs,
 *            where rhs = 1 for n==0 or n==25, rhs = 2 for 1 <= n <= 24, rhs =
 *            0 otherwise. The first 25 terms are generated automatically by
 *            this recurrence
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* memoised values */
static size_t cache_len = 0;    /* number of computed entries */
static size_t cache_cap = 0;    /* allocated capacity */

static void ensure_cache(idx_t up_to)
{
    /* Ensure we have space for indices 0 … up_to */
    if ((size_t)up_to + 1 > cache_cap) {
        size_t new_cap = (size_t)up_to + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t n = cache_len; n <= up_to; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 24 && n >= k; ++k) {
            sum += cache[n - k];
        }

        seq_t rhs;
        if (n == 0 || n == 25)
            rhs = 1;
        else if (n <= 24)
            rhs = 2;
        else
            rhs = 0;

        seq_t term = sum * (seq_t)44;
        if (n >= 25) {
            term -= cache[n - 25] * (seq_t)990;
        }
        term += rhs;

        cache[n] = term;
    }

    cache_len = (size_t)up_to + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
