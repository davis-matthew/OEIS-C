/*
 * A074357  Coefficient of q^3 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(1,3).
 * https://oeis.org/A074357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = coefficient of q^3 in ν(n), where * ν(0) = 1, * ν(1) =
 *            1, * ν(n) = ν(n‑1) + 3·(1 + q + … + q^(n‑2))·ν(n‑2) for n ≥ 2 *
 *            Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=30
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache: each entry stores the coefficients of q^0 … q^3 of ν(i).
       cache[i*4 + k] is the coefficient of q^k in ν(i). */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of ν‑values stored */

    /* Extend cache to include index n */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        /* realloc must be cast in C++ */
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            new_size * 4 * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failure */

        cache = new_cache;

        /* Initialise new entries to zero */
        for (idx_t i = cache_size; i < new_size; ++i) {
            for (int k = 0; k < 4; ++k) {
                cache[i * 4 + k] = 0;
            }
        }

        cache_size = new_size;

        /* Ensure base cases are set */
        if (cache_size >= 1) cache[0 * 4 + 0] = 1;      /* ν(0) = 1   */
        if (cache_size >= 2) cache[1 * 4 + 0] = 1;      /* ν(1) = 1   */

        /* Compute missing ν(i) for i = 2 … n */
        for (idx_t i = 2; i < cache_size; ++i) {
            for (int k = 0; k < 4; ++k) {
                /* ν(i‑1)_k */
                seq_t val = cache[(i - 1) * 4 + k];

                /* sum_{t=0}^{min(k, i‑2)} ν(i‑2)_{k‑t} */
                seq_t sum = 0;
                int max_t = k;
                if (max_t > (int)(i - 2)) max_t = (int)(i - 2);
                for (int t = 0; t <= max_t; ++t) {
                    sum += cache[(i - 2) * 4 + (k - t)];
                }

                cache[i * 4 + k] = val + sum * (seq_t)3; /* λ = 3 */
            }
        }
    }

    /* Return the q^3 coefficient */
    return cache[n * 4 + 3];
}
