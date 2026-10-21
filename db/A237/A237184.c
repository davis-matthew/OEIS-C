/*
 * A237184  Number of ordered ways to write n = (1+(n mod 2))*p + q with p, q,
 *          phi(p+1) - 1 and phi(q-1) + 1 all prime.
 * https://oeis.org/A237184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered ways to write * n = (1 + (n mod 2))*p +
 *            q * with p, q prime and phi(p+1) - 1 prime and phi(q-1) + 1
 *            prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Count the required representations for a given n. */
seq_t a(idx_t n)
{
    /* Work in native 64‑bit unsigned.  The index type is uint64_t. */
    uint64_t N = (uint64_t)n;
    if (N < 1) return (seq_t)0;          /* safety for n < 1 */

    /* Allocate temporary arrays up to N+1. */
    size_t sz = (size_t)(N + 2);          /* +2 to allow index N+1 */
    uint64_t *phi = (uint64_t*)malloc(sz * sizeof(uint64_t));
    bool      *is_prime = (bool*)malloc(sz * sizeof(bool));
    uint64_t *primes = (uint64_t*)malloc(sz * sizeof(uint64_t));
    if (!phi || !is_prime || !primes) {
        /* Allocation failure – return 0 (the function is not the
         * expression evaluator itself, so we can simply bail out). */
        free(phi); free(is_prime); free(primes);
        return (seq_t)0;
    }

    /* Initialise φ(i) = i and the primality array. */
    for (uint64_t i = 0; i <= N + 1; ++i) {
        phi[i] = i;
        is_prime[i] = true;
    }
    is_prime[0] = is_prime[1] = false;

    /* Simple sieve to fill primes and φ values. */
    size_t prime_cnt = 0;
    for (uint64_t i = 2; i <= N + 1; ++i) {
        if (is_prime[i]) {                    /* i is prime */
            primes[prime_cnt++] = i;
            /* Update φ for all multiples of i. */
            for (uint64_t j = i; j <= N + 1; j += i)
                phi[j] -= phi[j] / i;
            /* Mark composites for the sieve.  Avoid overflow of i*i. */
            if (i <= (N + 1) / i) {
                uint64_t start = i * i;
                for (uint64_t j = start; j <= N + 1; j += i)
                    is_prime[j] = false;
            }
        }
    }

    /* Count the admissible (p,q) pairs. */
    uint64_t k     = 1 + (N & 1);     /* 1 for even n, 2 for odd n */
    uint64_t limit = N / k;          /* maximal allowed p */
    uint64_t count = 0;

    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint64_t p = primes[idx];
        if (p > limit) break;

        uint64_t q = N - k * p;
        if (q < 2) continue;
        if (!is_prime[q]) continue;

        /* φ(p+1) and φ(q-1) are precomputed. */
        uint64_t val1 = phi[p + 1] - 1;            /* must be prime */
        if (val1 < 2 || !is_prime[val1]) continue;

        uint64_t val2 = phi[q - 1] + 1;            /* must be prime */
        if (val2 < 2 || !is_prime[val2]) continue;

        ++count;
    }

    /* Clean up. */
    free(phi);
    free(is_prime);
    free(primes);

    return (seq_t)count;
}
