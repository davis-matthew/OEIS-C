/*
 * A122489  Partial sums of A111939 (= number of primes < semiprime(n)).
 * https://oeis.org/A122489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=1..n} π( semiprime(k) - 1 ) * where semiprime(k)
 *            is the k‑th number having exactly two prime factors (counted
 *            with multiplicity)
 * Range:     u8 n <= 23, u16 n <= 479, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_prime_pi.h"

/* Helper to build the smallest prime factor array */
static void build_spf(uint64_t limit, uint32_t *spf)
{
    /* initialise to 0 */
    for (uint64_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {             /* i is prime */
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for indices < 1 */

    /* Initial estimate for the sieve upper bound */
    uint64_t limit = n * 20 + 1000;
    if (limit < 1000) limit = 1000;

    for (;;) {
        /* Allocate and build smallest prime factor array */
        uint32_t *spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        if (!spf) return (seq_t)0;
        build_spf(limit, spf);

        /* Allocate and build π(x) */
        seq_t *pi = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
        if (!pi) { free(spf); return (seq_t)0; }
        build_prime_pi(limit, spf, pi);

        /* Scan sequentially for semiprimes and accumulate */
        seq_t cumulative = 0;
        idx_t seen = 0;
        for (uint64_t num = 4; num <= limit; ++num) {
            uint64_t x = num;
            uint8_t count = 0;
            while (x > 1 && count <= 2) {
                uint32_t p = spf[x];
                while (x % p == 0) {
                    ++count;
                    x /= p;
                    if (count > 2) break;
                }
            }
            if (count == 2) { /* num is a semiprime */
                cumulative += pi[num - 1];
                if (++seen == n) {
                    /* Clean up before returning */
                    free(spf);
                    free(pi);
                    return cumulative;
                }
            }
        }

        /* Not enough semiprimes – increase limit and retry */
        free(spf);
        free(pi);
        limit <<= 1; /* double the limit for the next iteration */
    }
}
