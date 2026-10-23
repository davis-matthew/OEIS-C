/*
 * A169031  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n ≥ 1: a(n) = p_n + 16 * Σ_{i=1}^{min(23,n)}
 *            a(n-i) – 136 * a(n-24), where p_n = 1 if n = 0 or n = 24, = 2 if
 *            1 ≤ n ≤ 23, = 0 otherwise
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for already computed values. */
static seq_t *cache      = NULL;   /* array holding a(n) */
static idx_t  cache_len  = 0;      /* first index not yet computed + 1 */
static size_t cache_cap  = 0;      /* current allocated capacity */

/* Ensure the cache can hold indices up to 'need'. */
static void ensure_cap(idx_t need)
{
    size_t required = (size_t)(need + 1);
    if (required <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < required)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Compute a(n) using the recurrence, filling the cache as needed. */
static seq_t a_compute(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    /* Initialise the first value if the cache is empty. */
    if (cache_len == 0) {
        ensure_cap(0);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Fill the cache up to n. */
    for (idx_t k = cache_len; k <= n; ++k) {

        /* Sum of the last up to 23 terms. */
        seq_t sum = 0;
        idx_t m = (k < 23) ? k : 23;
        for (idx_t i = 1; i <= m; ++i)
            sum += cache[k - i];

        seq_t part1 = sum * (seq_t)16;
        seq_t part2 = 0;
        if (k >= 24)
            part2 = cache[k - 24] * (seq_t)136;

        seq_t p = 0;
        if (k <= 24) {
            p = (k == 0 || k == 24) ? (seq_t)1 : (seq_t)2;
        }

        cache[k] = p + part1 - part2;
    }

    cache_len = n + 1;
    return cache[n];
}

/* Public entry point required by the build system. */
seq_t a(idx_t n)
{
    return a_compute(n);
}
