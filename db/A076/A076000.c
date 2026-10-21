/*
 * A076000  a(n) = Product_{k=1..n} k/floor(n/k).
 * https://oeis.org/A076000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} k / floor(n/k)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute A076000: a(n) = n! / Prod_{k=1}^n floor(n/k)  */
seq_t a(idx_t n)
{
    /* offset 1: a(1) = 1 */
    if (n < 1) return (seq_t)0;

    size_t N = (size_t)n;

    /* Allocate arrays for sieve */
    size_t sz = N + 1;
    uint32_t *spf        = (uint32_t *)malloc(sz * sizeof(uint32_t));   /* smallest prime factor */
    uint32_t *primes     = (uint32_t *)malloc(sz * sizeof(uint32_t));   /* list of primes */
    uint32_t *primeIndex = (uint32_t *)malloc(sz * sizeof(uint32_t));   /* prime → index */
    if (!spf || !primes || !primeIndex) {
        free(spf); free(primes); free(primeIndex);
        return (seq_t)0;
    }

    for (size_t i = 0; i < sz; ++i) spf[i] = 0, primeIndex[i] = 0xFFFFFFFFu;

    size_t pc = 0;   /* number of primes found */
    for (size_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            primes[pc]   = (uint32_t)i;
            primeIndex[i] = (uint32_t)pc;
            ++pc;
            if (i * i <= N) {
                for (size_t j = i * i; j <= N; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    /* exponent arrays */
    size_t *numExp = (size_t *)malloc(pc * sizeof(size_t));
    size_t *denExp = (size_t *)malloc(pc * sizeof(size_t));
    if (!numExp || !denExp) {
        free(spf); free(primes); free(primeIndex);
        free(numExp); free(denExp);
        return (seq_t)0;
    }
    for (size_t i = 0; i < pc; ++i) numExp[i] = denExp[i] = 0;

    /* Numerator (prime exponents in n!) */
    for (size_t i = 0; i < pc; ++i) {
        uint32_t p = primes[i];
        size_t t   = N, e = 0;
        while (t) {
            t /= p;
            e += t;
        }
        numExp[i] = e;
    }

    /* Denominator: product of floor(N/k) for k=1..N */
    for (size_t k = 1; k <= N; ++k) {
        size_t m = N / k;
        while (m > 1) {
            uint32_t p = spf[m];
            size_t c   = 0;
            while (m % p == 0) {
                m /= p;
                ++c;
            }
            size_t idx = primeIndex[p];
            denExp[idx] += c;
        }
    }

    /* Construct result from prime differences */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < pc; ++i) {
        size_t exp = numExp[i] - denExp[i];
        for (size_t j = 0; j < exp; ++j)
            result = result * (seq_t)primes[i];
    }

    /* free memory */
    free(spf); free(primes); free(primeIndex);
    free(numExp); free(denExp);

    return result;
}
