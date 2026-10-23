/*
 * A170571  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1;
   for 1 ≤ n ≤ 46:  a(n) = 2 + 32 * Σ_{k=1}^{n} a(n-k);
   for n ≥ 47:      a(n) = 32 * Σ_{k=1}^{46} a(n-k) – 528 * a(n-47);
   This is equivalent to the rational generating function in the OEIS entry. */

#include <stdlib.h>

/* returns the nth term of A170571 */
seq_t a(idx_t n)
{
    /* static memoization buffer */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements stored */

    /* enlarge cache if we need more terms */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;          /* out‑of‑memory – return 0 as safe fallback */
        cache = new_cache;

        /* compute terms that are not yet in the cache */
        for (size_t i = cache_size; i < new_size; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            } else if (i <= 46) {
                seq_t sum = 0;
                for (size_t k = 1; k <= i; ++k)
                    sum += cache[i - k];
                val = 2 + 32 * sum;
            } else { /* i >= 47 */
                seq_t sum = 0;
                for (size_t k = 1; k <= 46; ++k)
                    sum += cache[i - k];
                val = 32 * sum - 528 * cache[i - 47];
            }
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
