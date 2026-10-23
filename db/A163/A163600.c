/*
 * A163600  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1, a(1) = 35, a(2) = 1190, a(3) = 40460, a(4) =
 *            1375640, * a(5) = 46771165. * For n ≥ 6: * a(n) = 33·a(n‑1) +
 *            33·a(n‑2) + 33·a(n‑3) + 33·a(n‑4) – 561·a(n‑5)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;   /* number of elements currently stored */

    /* Initial five values (plus a(5) for the first nonhomogeneous step). */
    static const seq_t init_vals[6] = {
        (seq_t)1,
        (seq_t)35,
        (seq_t)1190,
        (seq_t)40460,
        (seq_t)1375640,
        (seq_t)46771165
    };

    /* For n less than 6, just return the stored value. */
    if (n < 6)
        return init_vals[n];

    /* Ensure the cache has enough capacity. */
    if (n + 1 > cache_sz) {
        idx_t new_sz = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!new_cache) /* allocation failed – return 0 (unlikely). */
            return (seq_t)0;
        cache = new_cache;

        /* Initialise cache the first time. */
        if (cache_sz == 0) {
            for (idx_t i = 0; i < 6; ++i)
                cache[i] = init_vals[i];
            cache_sz = 6;
        }

        /* Compute remaining terms up to n. */
        for (idx_t i = cache_sz; i <= n; ++i) {
            /* a(i) = 33·a(i‑1)+33·a(i‑2)+33·a(i‑3)+33·a(i‑4)-561·a(i‑5) */
            seq_t val = 33 * cache[i-1] + 33 * cache[i-2] +
                        33 * cache[i-3] + 33 * cache[i-4] -
                        561 * cache[i-5];
            cache[i] = val;
        }
        cache_sz = new_sz;
    }

    return cache[n];
}
