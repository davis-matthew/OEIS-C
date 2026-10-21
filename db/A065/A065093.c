/*
 * A065093  Convolution of A000010 with itself.
 * https://oeis.org/A065093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} phi(k)*phi(n+1-k)
 * Range:     u8 n <= 14, u16 n <= 98, u32 n <= 3928, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Handle invalid/zero indices */
    if (n == 0)
        return 0;

    /* Static cache for Euler's totient values up to the maximum
     * value requested so far.  phi_arr[0] is unused but defined
     * for convenience.  "cached_max" is the largest index for which
     * phi_arr is known.  The array is recomputed whenever a larger
     * "n" is requested.  (Linear sieve, O(n) time.)  The totient
     * values are stored in a 64‑bit unsigned integer to avoid
     * overflow during their computation – the final convolution
     * result is accumulated into a seq_t variable. */
    static uint64_t *phi_arr = NULL;
    static idx_t      cached_max = 0;

    if ((idx_t)n > cached_max) {
        /* Allocate temporary arrays for the sieve */
        size_t   m = (size_t)n + 1;
        uint64_t *phi_new = (uint64_t*)malloc(m * sizeof(uint64_t));
        idx_t    *spf     = (idx_t*)calloc(m, sizeof(idx_t));
        idx_t    *primes  = (idx_t*)malloc(m * sizeof(idx_t));
        if (!phi_new || !spf || !primes) {
            /* Allocation failed – return 0 (the function is not
               expected to handle exceeded memory). */
            free(phi_new); free(spf); free(primes);
            return 0;
        }

        /* Initialize phi[0] and phi[1] */
        phi_new[0] = 0;
        if (m > 1)
            phi_new[1] = 1;

        size_t prime_cnt = 0;
        for (idx_t i = 2; i <= n; ++i) {
            if (spf[i] == 0) {            /* i is a prime */
                spf[i] = i;
                primes[prime_cnt++] = i;
                phi_new[i] = (uint64_t)i - 1;
            }
            for (size_t j = 0; j < prime_cnt; ++j) {
                idx_t p = primes[j];
                if (p > spf[i] || i * p > n) break;
                spf[i * p] = p;
                if (i % p == 0) {
                    /* p divides i */
                    phi_new[i * p] = phi_new[i] * (uint64_t)p;
                } else {
                    phi_new[i * p] = phi_new[i] * ((uint64_t)p - 1);
                }
            }
        }

        /* Free the temporary arrays and update the static cache. */
        free(phi_arr);
        free(spf);
        free(primes);
        phi_arr = phi_new;
        cached_max = n;
    }

    /* Convolution: a(n) = sum_{k=1..n} phi(k)*phi(n+1-k) */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint64_t a = phi_arr[k];
        uint64_t b = phi_arr[n + 1 - k];
        sum += (seq_t)(a * b);
    }
    return sum;
}
