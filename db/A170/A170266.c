/*
 * A170266  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = p_n + 15 * Σ_{k=1}^{min(n,40)} a(n-k) (1 ≤ n ≤
 *            40) a(41)= 1 + 15*Σ_{k=1}^{40} a(41-k) – 120*a(0) a(n) = 15 *
 *            Σ_{k=1}^{40} a(n-k) – 120 * a(n-41) (n ≥ 42) Here p_n = 1 for n
 *            = 0 or 41, p_n = 2 for 1 ≤ n ≤ 40, and p_n = 0 otherwise. This
 *            recurrence is equivalent to the rational gene...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* cache of already computed values */
static size_t cached = 0;            /* number of entries stored in cache */

/* extend the cache so that it holds values up to index n */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cached)
        return;                     /* already available */

    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache && new_size > 0)
        abort();                    /* out of memory */
    cache = new_cache;

    /* compute new entries */
    for (size_t i = cached; i < new_size; ++i)
    {
        seq_t sum = 0;              /* Σ a(i-k) for k = 1..min(i,40) */
        if (i > 0)
        {
            size_t jmax = (i >= 40 ? 40 : i);
            for (size_t j = 1; j <= jmax; ++j)
                sum += cache[i - j];
        }

        /* p_n */
        seq_t term;
        if (i == 0)
            term = (seq_t)1;
        else if (i <= 40)
            term = (seq_t)2;
        else if (i == 41)
            term = (seq_t)1;
        else
            term = (seq_t)0;

        /* add 15 * sum */
        term += (seq_t)15 * sum;

        /* subtract 120 * a(i-41) for i ≥ 41 */
        if (i >= 41)
            term -= (seq_t)120 * cache[i - 41];

        cache[i] = term;
    }

    cached = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
