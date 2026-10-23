/*
 * A169415  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/*
 * Formula:  a(0) = 1
 *           a(n) = 16 * Σ_{k=1}^{min(31,n-1)} a(n-k) + c(n) – 136 * a(n-32),
 *                where
 *                c(n) = 2 for 1 ≤ n ≤ 31,
 *                c(32) = 1,
 *                c(n) = 0 otherwise.
 *
 * This implements the recurrence derived from the generating function
 * of OEIS sequence A169415.
 */
static seq_t *cache = NULL;        /* cached values a(0)…a(max_cached)   */
static size_t cache_cap = 0;       /* allocated size of *cache          */
static idx_t  max_cached = 0;      /* largest index that has a value    */
static bool   initialized = false; /* cache initialisation flag         */

/* Ensure *cache has capacity for indices up to n inclusive.  */
static void ensure_capacity(idx_t n)
{
    if (cache_cap <= (size_t)n)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                    /* allocation failed – abort  */
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main entry point – memoised computation of a(n).  */
seq_t a(idx_t n)
{
    /* Initialise cache on first call.  */
    if (!initialized)
    {
        ensure_capacity(1);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        max_cached = 0;
        initialized = true;
    }

    /* Return cached value if already computed.  */
    if (n <= max_cached)
        return cache[n];

    ensure_capacity(n);

    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t k = 1; k <= 31 && i >= k; ++k)
            sum += cache[i - k];

        seq_t coeff = 0;
        if (i == 32)
            coeff = (seq_t)1;
        else if (i <= 31)
            coeff = (seq_t)2;   /* for 1 ≤ i ≤ 31 */

        seq_t val = (seq_t)16 * sum + coeff;
        if (i >= 32)
            val -= (seq_t)136 * cache[i - 32];

        cache[i] = val;
    }

    max_cached = n;
    return cache[n];
}
