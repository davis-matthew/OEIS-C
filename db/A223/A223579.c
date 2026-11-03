/*
 * A223579  Number of n X n 0..3 arrays with antidiagonals unimodal.
 * https://oeis.org/A223579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = U(n) * Π_{i=1}^{n-1} U(i)^2 with U(z) =
 *            (z^6+15*z^5+115*z^4+405*z^3+964*z^2+660*z+720)/720
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Compute U(k) = (k^6+15*k^5+115*k^4+405*k^3+964*k^2+660*k+720)/720
   using Horner's rule to avoid intermediate overflow */
static seq_t compute_U(idx_t k)
{
    seq_t K = (seq_t)k;
    seq_t res = (seq_t)1;
    res = res * K + (seq_t)15;
    res = res * K + (seq_t)115;
    res = res * K + (seq_t)405;
    res = res * K + (seq_t)964;
    res = res * K + (seq_t)660;
    res = res * K + (seq_t)720;
    return res / (seq_t)720;   /* division is exact for integer k ≥ 1 */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; value for n=0 is unused */

    /* Cache previously computed values to accelerate repeated calls */
    static seq_t *cache = NULL;    /* a(1), a(2), ..., a(cache_size) */
    static idx_t cache_size = 0;   /* number of values cached */

    /* Extend cache if necessary */
    if (n > cache_size)
    {
        idx_t new_size = n;
        seq_t *new_cache = mem_resize(seq_t, cache, new_size);
        if (!new_cache) /* allocation failed */
            return (seq_t)0;
        cache = new_cache;

        /* Compute all new values from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            seq_t U_cur = compute_U(i);
            if (i == 1)
            {
                cache[0] = U_cur;               /* a(1) = U(1) */
            }
            else
            {
                seq_t prev = cache[i - 2];      /* a(i-1) */
                seq_t U_prev = compute_U(i - 1);/* U(i-1) */
                seq_t val = prev * U_prev;     /* intermediate product */
                val = val * U_cur;              /* final a(i) = a(i-1)*U(i-1)*U(i) */
                cache[i - 1] = val;
            }
        }
        cache_size = n;
    }

    return cache[n - 1];
}
