/*
 * A078767  Let f(n) = A003434(n) be the number of iterations of phi needed to
 *          reach 1. Then a(n) = max(f(1), f(2), ..., f(n)).
 * https://oeis.org/A078767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max_{1 <= k <= n} f(k) where f(k) is the number of
 *            iterations of Euler's totient function before reaching 1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function keeps a cache of phi values and iteration counts.
    It expands the cache when a larger n is requested.
    The code works for both C11 (malloc/free) and C++17
    (operator new[]/delete[]). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache state */
    static idx_t cache_len = 0;      /* current cache size (n+1) */
    static seq_t *phi = NULL;        /* phi[i]  = Euler's totient of i */
    static seq_t *f   = NULL;        /* f[i]    = iteration count to 1 */

    /* Expand cache if needed */
    if (n + 1 > cache_len) {
        /* Free previous cache */
        if (phi) {
#ifdef __cplusplus
            delete[] phi;
#else
            free(phi);
#endif
        }
        if (f) {
#ifdef __cplusplus
            delete[] f;
#else
            free(f);
#endif
        }

        /* Allocate new cache */
        cache_len = n + 1;

#ifdef __cplusplus
        phi = new (std::nothrow) seq_t[cache_len];
        f   = new (std::nothrow) seq_t[cache_len];
#else
        phi = (seq_t *)malloc(cache_len * sizeof(seq_t));
        f   = (seq_t *)malloc(cache_len * sizeof(seq_t));
#endif
        if (!phi || !f) {
            /* Allocation failed – treat as zero result */
            return (seq_t)0;
        }

        /* Initialise phi[i] = i */
        for (idx_t i = 0; i < cache_len; ++i) {
            phi[i] = (seq_t)i;
        }

        /* Sieve to compute phi values up to n */
        for (idx_t p = 2; p < cache_len; ++p) {
            if (phi[p] == (seq_t)p) {      /* p is prime */
                seq_t pr = (seq_t)p;
                for (idx_t j = p; j < cache_len; j += p) {
                    phi[j] -= phi[j] / pr;
                }
            }
        }

        /* Compute iteration counts f[i] */
        f[1] = 0;
        for (idx_t i = 2; i < cache_len; ++i) {
            f[i] = f[(idx_t)phi[i]] + (seq_t)1;
        }
    }

    /* Find maximum f(k) for 1 <= k <= n */
    seq_t max_val = f[1];
    for (idx_t i = 2; i <= n; ++i) {
        if (f[i] > max_val) {
            max_val = f[i];
        }
    }
    return max_val;
}
