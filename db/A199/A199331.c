/*
 * A199331  Number of ordered ways of writing n as the sum of two semiprimes.
 * https://oeis.org/A199331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=1}^{n‑1} c(k)·c(n‑k) where c(k)=1 if k is a
 *            semiprime, 0 otherwise
 * Range:     u8 n <= 1799, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the number of ordered ways to write n as the sum of two semiprimes. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* no pairs for n<2 */

    /* ---------- static cache for repeated calls ---------- */
    static idx_t max_n   = 0;
    static uint8_t *semip = NULL;      /* semiprime flag: 1 if i is semiprime */
    static uint32_t *spf   = NULL;     /* smallest prime factor of i */

    if (n > max_n) {                   /* enlarge precomputation if needed */
        idx_t new_n = n;
        size_t sz = (size_t)new_n + 1; /* include index 0 */

        /* Allocate new arrays */
        uint32_t *new_spf   = (uint32_t*)malloc(sz * sizeof(uint32_t));
        uint8_t  *new_semip = (uint8_t*)malloc(sz * sizeof(uint8_t));
        if (!new_spf || !new_semip) {      /* allocation failure: safe return */
            free(new_spf); free(new_semip);
            return 0;
        }

        /* Initialise spf array */
        for (size_t i = 0; i < sz; ++i)
            new_spf[i] = 0;

        /* Sieve for smallest prime factor */
        for (idx_t p = 2; p <= new_n / p; ++p) {   /* avoid overflow */
            if (new_spf[p] == 0) {                 /* p is prime */
                for (idx_t j = p * p; j <= new_n; j += p) {
                    if (new_spf[j] == 0)
                        new_spf[j] = (uint32_t)p;
                }
            }
        }
        /* Mark primes themselves */
        for (idx_t i = 2; i <= new_n; ++i)
            if (new_spf[i] == 0)
                new_spf[i] = (uint32_t)i;

        /* Compute semiprime indicator */
        new_semip[0] = new_semip[1] = 0;
        for (idx_t i = 2; i <= new_n; ++i)
            new_semip[i] = 0;
        for (idx_t i = 4; i <= new_n; ++i) {
            uint32_t p   = new_spf[i];
            uint32_t rest = i / p;
            if (new_spf[rest] == rest)          /* rest is prime      */
                new_semip[i] = 1;
        }

        /* Free old arrays and store the new ones */
        free(spf);
        free(semip);
        spf   = new_spf;
        semip = new_semip;
        max_n = new_n;
    }

    /* ---------- convolution: count ordered pairs ---------- */
    seq_t result = 0;
    for (idx_t k = 1; k < n; ++k) {
        if (semip[k] && semip[n - k])
            result += (seq_t)1;
    }

    return result;
}
