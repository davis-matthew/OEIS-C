/*
 * A126520  Number of base 25 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of length‑n base‑25 strings whose adjacent digits differ by ≤4 */
/*  a(0)=1, a(1)=25, a(2)=205, a(3)=1725, …  computed by dynamic programming. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* stores a(n) up to the maximum computed n */
static size_t cache_cap = 0;     /* allocated size of cache */
static idx_t max_n_computed = 0;

/* fixed size arrays for DP; base is 25 */
static seq_t dp_prev[25];
static seq_t dp_curr[25];

/* Ensure cache can store values up to n (inclusive). */
static void cache_ensure(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Handle n = 0 separately. */
    if (n == 0) {
        /* Ensure cache has space for index 0 */
        cache_ensure(0);
        if (cache_cap > 0 && cache[0] != 0)
            return cache[0];
        cache[0] = (seq_t)1;
        max_n_computed = 0;
        return cache[0];
    }

    /* Make sure cache can hold up to n */
    cache_ensure(n);
    if (n <= max_n_computed)
        return cache[n];

    /* First time initialization for n >= 1 */
    if (max_n_computed == 0) {
        /* dp_prev represents length 1 strings: all digits 0..24 are allowed */
        for (int d = 0; d < 25; ++d)
            dp_prev[d] = (seq_t)1;

        /* total for length 1 */
        seq_t total = (seq_t)0;
        for (int d = 0; d < 25; ++d)
            total += dp_prev[d];
        cache[1] = total;
        max_n_computed = 1;

        /* cache[0] (empty string) already set earlier */
        if (n == 1)
            return cache[1];
    }

    /* Compute values for lengths > max_n_computed up to n */
    for (idx_t len = max_n_computed + 1; len <= n; ++len)
    {
        /* For each digit d, sum dp_prev[k] over |k-d| <= 4 */
        for (int d = 0; d < 25; ++d) {
            seq_t sum = (seq_t)0;
            int start = d - 4; if (start < 0) start = 0;
            int end   = d + 4; if (end   > 24) end   = 24;
            for (int k = start; k <= end; ++k)
                sum += dp_prev[k];
            dp_curr[d] = sum;
        }

        /* Total number of strings of current length */
        seq_t total = (seq_t)0;
        for (int d = 0; d < 25; ++d)
            total += dp_curr[d];
        cache[(size_t)len] = total;

        /* Prepare for next iteration: dp_curr becomes dp_prev */
        for (int d = 0; d < 25; ++d)
            dp_prev[d] = dp_curr[d];
    }

    max_n_computed = n;
    return cache[n];
}
