/*
 * A161809  G.f.: A(x) = exp( Sum_{n>=1} 3*A038500(n) * x^n/n ), where A038500
 *          is the highest power of 3 dividing n.
 * https://oeis.org/A161809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = Σ_{k=0}^{⌊n/3⌋} c_{n-3k} · a(k) where c_0 = 1
 *            and c_m = 3·m for m ≥ 1. This follows from the functional
 *            equation A(x) = A(x³)*(1+x+x²)/(1‑x)²
 * Range:     u8 n <= 10, u16 n <= 43, u32 n <= 267, u64 n <= 2951,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Compute the nth term of sequence A161809 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;           /* memoization buffer          */
    static idx_t cache_size = 0;          /* number of computed entries   */

    if (n + 1 > cache_size) {
        /* Allocate (or grow) the cache to hold values up to n */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache,
                                     new_size * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */

        /* First term is defined for all n */
        if (cache_size == 0) {
            tmp[0] = (seq_t)1;
            cache_size = 1;
        }

        /* Compute new terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; (3 * k) <= i; ++k) {
                idx_t m = i - 3 * k;
                seq_t coeff = (m == 0) ? (seq_t)1
                                         : (seq_t)(3 * m);
                sum = sum + coeff * tmp[k];
            }
            tmp[i] = sum;
        }

        cache = tmp;
        cache_size = n + 1;
    }

    return cache[n];
}
