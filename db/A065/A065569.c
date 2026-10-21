/*
 * A065569  Smallest k such that omega(n+k) = omega(k).
 * https://oeis.org/A065569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k ≥ 1 : ω(n+k) = ω(k)} where ω(m) is the number of
 *            distinct prime factors of m
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static tables for smallest prime factor and omega values */
static unsigned int *spf = NULL;          /* spf[i] = smallest prime factor of i */
static unsigned char *omega = NULL;       /* omega[i] = number of distinct prime factors of i */
static size_t spf_limit = 0;

/* Ensure spf and omega tables cover numbers up to 'limit' (inclusive). */
static void ensure_sieve(size_t limit)
{
    if (limit <= spf_limit)
        return;

    /* Resize tables */
    spf = (unsigned int*)realloc(spf, (limit + 1) * sizeof(unsigned int));
    omega = (unsigned char*)realloc(omega, (limit + 1) * sizeof(unsigned char));

    /* Initialize tables to 0 */
    for (size_t i = 0; i <= limit; ++i) {
        spf[i] = 0;
        omega[i] = 0;
    }

    /* Simple sieve to fill spf */
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = (unsigned int)i;
            if ((unsigned long long)i * i <= limit) {
                size_t start = i * i;
                for (size_t j = start; j <= limit; j += i) {
                    if (spf[j] == 0)
                        spf[j] = (unsigned int)i;
                }
            }
        }
    }

    /* Compute omega values */
    omega[0] = omega[1] = 0;
    for (size_t i = 2; i <= limit; ++i) {
        unsigned int p = spf[i];
        size_t x = i / p;
        while (x % p == 0)
            x /= p;
        omega[i] = omega[x] + 1;
    }

    spf_limit = limit;
}

/* Main function required by the problem */
seq_t a(idx_t n)
{
    if (n == 0)   /* Offset is 1, but guard against zero */
        return (seq_t)1;

    uint64_t N = (uint64_t)n;

    for (uint64_t k = 1;; ++k) {
        uint64_t target = N + k;
        ensure_sieve((size_t)target);       /* Ensure tables cover n + k and k */
        unsigned char omega_target = omega[target];
        unsigned char omega_k = omega[k];
        if (omega_target == omega_k)
            return (seq_t)k;
    }
}
