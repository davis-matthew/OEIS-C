/*
 * A126506  Number of base 11 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Let f[0](d) = 1 for all digits d = 0..10.
 * For n ≥ 1,  f[n+1](d) = Σ_{k=max(0,d-4)}^{min(10,d+4)} f[n](k).
 * Then a(n) = Σ_{d=0}^{10} f[n](d).
 *
 * Initial values: a(0) = 1, a(1) = 11.
 * This implements the recurrence for the number of base‑11 n‑digit
 * strings with adjacent digits differing by at most 4.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed a(n) values */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;     /* last index for which cache is valid */

    /* static arrays to hold f[n](d) for the previous and current n */
    static seq_t prev[11];
    static seq_t cur[11];

    /* If this is the first call, initialise the cache for n=0 and n=1 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure: return 0 (unlikely) */
        cache[0] = (seq_t)1;           /* a(0) */
        cache[1] = (seq_t)11;          /* a(1) */

        /* initialise f[1](d) = 1 for all digits d */
        for (idx_t d = 0; d < 11; ++d) {
            prev[d] = (seq_t)1;
        }
        max_computed = 1;
    }

    /* If the requested n is already computed, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Extend the cache up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return a(max_computed); /* in case realloc fails */
    cache = new_cache;

    /* Compute a(m) for m = max_computed + 1 .. n */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        /* compute cur[d] = Σ_{k=max(0,d-4)}^{min(10,d+4)} prev[k] */
        for (idx_t d = 0; d < 11; ++d) {
            seq_t sum = 0;
            idx_t k_start = (d >= 4) ? (d - 4) : 0;
            idx_t k_end   = (d + 4 < 11) ? (d + 4) : 10;
            for (idx_t k = k_start; k <= k_end; ++k) {
                sum += prev[k];
            }
            cur[d] = sum;
        }

        /* a(m) = Σ_{d=0}^{10} cur[d] */
        seq_t total = 0;
        for (idx_t d = 0; d < 11; ++d) {
            total += cur[d];
        }
        cache[m] = total;

        /* prepare for next iteration: copy cur to prev */
        for (idx_t d = 0; d < 11; ++d) {
            prev[d] = cur[d];
        }
    }

    max_computed = n;
    return cache[n];
}
