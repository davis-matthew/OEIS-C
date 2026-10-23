/*
 * A167537  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) satisfies the linear recurrence
 *   a(0)      = 1
 *   a(n) = 2 + 37·(a(n-1)+a(n-2)+…+a(n-13))          for 1 ≤ n ≤ 13
 *   a(14) = 1 + 37·(a(13)+…+a(1)) - 703·a(0)
 *   a(n) = 37·(a(n-1)+…+a(n-13)) - 703·a(n-14)       for n ≥ 15
 *
 * This recurrence follows from the generating function
 *   (t^14 + 2*t^13 + … + 2*t + 1) / (703*t^14 - 37*t^13 - … - 37*t + 1).
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache     = NULL;   /* computed terms */
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Allocate or grow the cache to at least `need` elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);          /* out of memory */
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    free(cache);
    cache     = new_cache;
    cache_cap = new_cap;
}

/* Compute term a(n) using the recurrence. */
seq_t a(idx_t n)
{
    /* Ensure recurrence constants (p-values) are defined for n ≤ 14. */
    static const seq_t p_terms[15] = {
        1, /* p[0] */
        2, /* p[1] */
        2, /* p[2] */
        2, /* p[3] */
        2, /* p[4] */
        2, /* p[5] */
        2, /* p[6] */
        2, /* p[7] */
        2, /* p[8] */
        2, /* p[9] */
        2, /* p[10] */
        2, /* p[11] */
        2, /* p[12] */
        2, /* p[13] */
        1  /* p[14] */
    };

    /* Grow cache to include n. */
    grow_cache((size_t)n + 1);

    /* If already computed, return. */
    if ((idx_t)n < (idx_t)cache_len)
        return cache[(size_t)n];

    /* Compute terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        /* Sum of the previous up to 13 terms. */
        seq_t sum13 = 0;
        size_t limit = i < 13 ? i : 13;
        for (size_t k = 1; k <= limit; ++k)
            sum13 += cache[i - k];

        seq_t prev14 = (i >= 14) ? cache[i - 14] : 0;
        seq_t p = (i <= 14) ? p_terms[i] : 0;
        seq_t term = p + (seq_t)37 * sum13 - (seq_t)703 * prev14;
        cache[i] = term;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
