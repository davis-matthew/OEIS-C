/*
 * A232353  Number of ways to write n = k + m with k > 0 and m > 0 such that p
 *          = prime(k) + phi(m) and p*(p+1) - prime(p) are both prime, where
 *          phi(.) is Euler's totient function.
 * https://oeis.org/A232353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of decompositions n = k + m (k>0, m>0) such that p
 *            = prime(k) + phi(m) is prime and p*(p+1) - prime(p) is also
 *            prime. Computation is by brute‑force search over k=1..n-1 with a
 *            pre‑computed list of primes
 * Range:     u8 n <= 9, u16 n <= 59, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   calc_phi, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/calc_phi.h"

/* Prime list storage */
static bool initialized = false;
static uint32_t *prime_list = NULL;   /* primes in ascending order */
static size_t   prime_count = 0;

/* Pre‑compute prime list up to a fixed limit */
static void init_primes(void)
{
    if (initialized) return;

    /* Safe upper bound for the 120 000‑th prime (≈2 million) */
    const uint32_t LIMIT = 2500000U;

    uint8_t *sieve = (uint8_t*)malloc(LIMIT + 1);
    memset(sieve, 1, LIMIT + 1);
    sieve[0] = sieve[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (!sieve[i]) continue;
        for (uint32_t j = i * i; j <= LIMIT; j += i)
            sieve[j] = 0;
    }

    size_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) ++cnt;

    prime_list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    prime_count = cnt;

    size_t pos = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) prime_list[pos++] = i;

    free(sieve);
    initialized = true;
}

seq_t a(idx_t n)
{
    init_primes();

    seq_t count = 0;

    if (n < 2) return 0;  /* no positive k, m with sum 1 */

    for (idx_t k = 1; k < n; ++k) {
        idx_t m = n - k;                        /* m > 0 */

        /* prime(k)    = k-th prime (1‑based) */
        seq_t pk = prime_list[(size_t)k - 1];

        /* phi(m)      */
        seq_t phi_m = calc_phi((seq_t)m);

        seq_t p = pk + phi_m;                   /* candidate prime */

        if (!isPrimeGeneric(p))
            continue;

        /* p must be ≤ number of precomputed primes */
        if (p > (seq_t)prime_count)
            continue;

        size_t idx = (size_t)((uint64_t)p - 1); /* p‑th prime */
        seq_t prime_of_p = prime_list[idx];

        seq_t val = p * (p + 1) - prime_of_p;
        if (isPrimeGeneric(val))
            count += 1;
    }

    return count;
}
