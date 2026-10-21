/*
 * A092904  Number of decimal digits in the denominator of the Bernoulli
 *          number B(2n).
 * https://oeis.org/A092904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = number of decimal digits of the denominator of the
 *            Bernoulli number B(2n) (a(0) = 1). The denominator is given by
 *            von Staudt–Clausen: denom(B(2n)) = ∏{ p prime | p-1 divides 2n }
 *            p
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The number of decimal digits of that product is simply ⌊log10(product)⌋+1. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* maximum n that the problem needs to support (≈10000) */
#define MAX_N      10000
/* smallest prime upper bound needed: 2*MAX_N+1 */
#define PRIME_LIMIT (2 * MAX_N + 1)

/* Static storage for the primes up to PRIME_LIMIT */
static unsigned int primes[2500];  /* enough to hold all primes <= 20001 */
static size_t prime_count = 0;
static int primes_initialised = 0;

/* One‑time sieve of Eratosthenes generating all primes <= PRIME_LIMIT. */
static void init_primes(void)
{
    if (primes_initialised) return;
    unsigned char *sieve = (unsigned char *)malloc(PRIME_LIMIT + 1);
    if (!sieve) return;  /* allocation failure – unlikely */
    memset(sieve, 1, PRIME_LIMIT + 1);
    sieve[0] = sieve[1] = 0;
    for (unsigned int i = 2; (unsigned long long)i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (unsigned int j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;
        }
    }
    for (unsigned int i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) primes[prime_count++] = i;
    free(sieve);
    primes_initialised = 1;
}

/* Return the sequence value a(n). */
seq_t a(idx_t n)
{
    /* special case: B0 = 1, denominator 1, 1 digit */
    if (n == 0u) return (seq_t)1;

    init_primes();

    unsigned long long two_n = (unsigned long long)n * 2ull;
    double log_sum = 0.0;

    /* Sum the logs of all primes p such that p-1 divides 2n.  p-1 ≤ 2n, so
       we only need to consider primes up to two_n + 1. */
    for (size_t i = 0; i < prime_count && primes[i] <= two_n + 1; ++i) {
        unsigned int p = primes[i];
        if (two_n % (p - 1) == 0u) {
            log_sum += log10((double)p);
        }
    }

    /* Number of decimal digits = floor(log10(product)) + 1.  A tiny epsilon
       protects against rare floating point rounding errors. */
    unsigned long long digits = (unsigned long long)floor(log_sum + 1e-12) + 1ull;
    return (seq_t)digits;
}
