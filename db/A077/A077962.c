/*
 * A077962  Expansion of 1/(1+x^2+x^3).
 * https://oeis.org/A077962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(a(n-2) + a(n-3)) a(0) = 1, a(1) = 0, a(2) = -1
 *            Recurrence derived from (1 + x² + x³) * Σ a(n) xⁿ = 1
 * Range:     i8 n <= 26, i16 n <= 55, i32 n <= 114, i64 n <= 231,
 *            i128 n <= 461, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* table of computed coefficients */
static size_t cache_len = 0;     /* number of entries currently stored */
static size_t cache_cap = 0;     /* allocated capacity of cache array */

/* Ensure the cache array can hold at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1U : 4;
    while (new_cap < need)
        new_cap <<= 1U;

    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_arr;
    cache_cap = new_cap;
}

/* Initialise the first three known terms (if not already done) */
static void initialise_cache(void)
{
    if (cache_len > 0)
        return;                 /* already initialised */

    ensure_cache_cap(3);
    cache[0] = (seq_t)1;         /* a(0) = 1  */
    cache[1] = (seq_t)0;         /* a(1) = 0  */
    cache[2] = -(seq_t)1;        /* a(2) = -1 */
    cache_len = 3;
}

/* Compute and return a(n) */
seq_t a(idx_t n)
{
    ensure_cache_cap((size_t)n + 1);
    initialise_cache();

    /* If we have already computed up to 'n', just return it */
    if (n < (idx_t)cache_len)
        return cache[(size_t)n];

    /* Compute new terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        seq_t new_val;
        if (i == 0)              /* should not happen because cache_len starts at 3 */
            new_val = (seq_t)1;
        else if (i == 1)
            new_val = (seq_t)0;
        else if (i == 2)
            new_val = -(seq_t)1;
        else
        {
            /* a(i) = -(a(i-2) + a(i-3)) */
            seq_t sum = cache[i - 2] + cache[i - 3];
            new_val = -sum;
        }
        cache[i] = new_val;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
