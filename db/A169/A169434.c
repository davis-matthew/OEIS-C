/*
 * A169434  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169434: Number of reduced words of length n in a Coxeter group on 37 generators.
 * The sequence satisfies the linear recurrence
 *   a(0) = 1
 *   for 1 ≤ n ≤ 31:  a(n) = 2 + 35·∑_{k=1}^{n} a(n‑k)
 *   for n = 32:      a(32) = 1 + 35·∑_{k=1}^{31} a(32‑k) – 630·a(0)
 *   for n ≥ 33:      a(n) = 35·∑_{k=1}^{31} a(n‑k) – 630·a(n‑32)
 *
 * This implementation memoises the values in a static array so that arbitrary
 * calls in any order remain efficient (linear work overall).
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* memoised values */
    static idx_t cache_size = 0;    /* number of values stored (next index to fill) */
    static size_t cache_cap = 0;    /* capacity of allocated array */

    /* Return if already computed */
    if (n < cache_size)
        return cache[n];

    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) /* allocation failure – abort */
            abort();
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* Initialise first value if not yet done */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        /* Sum of the last up to 31 terms */
        for (idx_t k = 1; k <= 31 && i >= k; ++k)
            sum += cache[i - k];

        seq_t term = (seq_t)35 * sum;
        if (i >= 32)
            term -= (seq_t)630 * cache[i - 32];

        seq_t coeff = 0;
        if (i == 32)      coeff = 1;
        else if (i <= 31 && i >= 1) coeff = 2;

        cache[i] = coeff + term;
    }
    cache_size = n + 1;
    return cache[n];
}
