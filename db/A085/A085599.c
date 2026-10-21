/*
 * A085599  Number of pairs of coprimes (n-i,n+i), 1<i<n.
 * https://oeis.org/A085599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(2*n)/2 – ((-1)^n + 1)/2 (Euler’s totient φ)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime sieve for trial‑division (used to compute φ(2*n)) */
static uint32_t *primes = NULL;
static size_t     prime_count = 0;

/* Initialise a prime list that is large enough for n up to 10000 */
static void init_primes(void)
{
    if (primes != NULL) return;                /* already initialised */

    /* 2*10000 < 20000 ⇒ sqrt ≈ 143; the limit below is comfortably large. */
    const unsigned int limit = 50000;
    unsigned char *is_prime = (unsigned char *)calloc(limit + 1, 1);
    if (!is_prime) return;                     /* allocation failure */

    for (unsigned int i = 2; i <= limit; ++i) is_prime[i] = 1;
    for (unsigned int i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (unsigned int j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    size_t cnt = 0;
    for (unsigned int i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes) { free(is_prime); return; }

    size_t idx = 0;
    for (unsigned int i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = i;

    prime_count = cnt;
    free(is_prime);
}

/* Compute Euler’s totient φ(m) for 64‑bit unsigned integers. */
static uint64_t phi_uint64(uint64_t m)
{
    uint64_t result = m;
    uint64_t temp   = m;

    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            result -= result / p;
            do { temp /= p; } while (temp % p == 0);
        }
    }
    if (temp > 1) result -= result / temp;    /* remaining prime factor */
    return result;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) { init_primes(); initialized = true; }

    /* φ(2*n) fits in 64‑bit because 2*n < 2^65. */
    uint64_t phi2n = phi_uint64(2ULL * n);
    seq_t phi_half    = (seq_t)(phi2n / 2);
    seq_t adjust      = ((n & 1U) == 0U) ? 1U : 0U;        /* ((-1)^n + 1)/2 */

    return phi_half - adjust;
}
