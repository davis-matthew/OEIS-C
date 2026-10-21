/*
 * A229138  Number of solutions to Sum_{i=1...n} x_i^2 == 1 (mod 8) with x_i
 *          in 0..7.
 * https://oeis.org/A229138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) counts n‑tuples (x1…xn) with each xi∈{0,…,7} such that ∑xi²
 *            ≡ 1 (mod 8). Computed by dynamic programming on the 8 residue
 *            classes of the partial sums
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the surrounding build. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache of a(n) values */
    static idx_t cache_size = 0;         /* allocated size of cache */
    static idx_t max_computed = 0;       /* largest n for which cache was filled */
    static seq_t v[8];                   /* current counts for residue class 0..7 */
    static bool initialized = false;     /* one‑time initialization flag */

    if (!initialized) {
        initialized = true;
        cache_size = 1;                  /* reserve index 0 (unused) */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed  */
        cache[0] = 0;                    /* dummy for n == 0 */

        /* vector for n = 0  (empty tuple). Only sum 0 is possible. */
        for (int k = 0; k < 8; ++k) v[k] = 0;
        v[0] = 1;
        max_computed = 0;
    }

    /* Ensure cache can hold the requested index. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* Extend the precomputation up to n if necessary. */
    if (n > max_computed) {
        seq_t newv[8];
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* compute counts for i from those of i-1 */
            for (int k = 0; k < 8; ++k) {
                int km1 = (k + 7) & 7;          /* (k-1) mod 8 */
                int km4 = (k + 4) & 7;          /* (k-4) mod 8 */
                seq_t term0 = (seq_t)2 * v[k];
                seq_t term1 = (seq_t)4 * v[km1];
                seq_t term2 = (seq_t)2 * v[km4];
                newv[k] = term0 + term1 + term2;
            }
            for (int k = 0; k < 8; ++k) v[k] = newv[k];
            cache[i] = v[1];   /* a(i) is the count of sum ≡ 1 */
        }
        max_computed = n;
    }

    return cache[n];
}
