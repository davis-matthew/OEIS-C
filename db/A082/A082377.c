/*
 * A082377  Numbers k such that rad(k) - phi(k) <= 1, where rad = A007947 and
 *          phi = A000010.
 * https://oeis.org/A082377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A082377:  Numbers k such that rad(k) - phi(k) ≤ 1.
    The function generates integers k in increasing order and
    returns the nth such integer (n ≥ 1).  It precomputes a
    list of primes up to a fixed limit and factors each k by
    trial division using that list.  The implementation works
    for any build with seq_t and idx_t defined as required. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIEVE_LIMIT 200000   /* enough for the first 10 000 terms */

/* forward declarations for types supplied by the build */
extern seq_t a(idx_t n);

/* static data for the prime sieve */
static uint32_t *primes = NULL;   /* list of primes */
static size_t primes_cnt = 0;     /* number of primes */

/* Initialise the prime sieve only once. */
static void init_primes(void)
{
    if (primes != NULL)
        return;  /* already initialized */

    /* Simple Eratosthenes sieve up to SIEVE_LIMIT */
    size_t n = SIEVE_LIMIT;
    bool *sieve = (bool *)malloc((n + 1) * sizeof(bool));
    if (!sieve)
        return;  /* out of memory, will fail gracefully */

    for (size_t i = 0; i <= n; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= n; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= n; j += i)
                sieve[j] = false;
        }
    }

    /* Count primes */
    primes_cnt = 0;
    for (size_t i = 2; i <= n; ++i)
        if (sieve[i]) ++primes_cnt;

    /* Allocate and store primes */
    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) {
        free(sieve);
        return;  /* out of memory */
    }
    size_t idx = 0;
    for (size_t i = 2; i <= n; ++i)
        if (sieve[i])
            primes[idx++] = (uint32_t)i;

    free(sieve);
}

/* Compute rad(k) and phi(k) using the precomputed prime list. */
static void compute_rad_phi(uint64_t k, uint64_t *rad, uint64_t *phi)
{
    *rad = 1;
    *phi = k;
    uint64_t temp = k;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > temp)
            break;
        if (temp % p == 0) {
            *rad *= p;
            while (temp % p == 0)
                temp /= p;
            *phi -= *phi / p;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        *rad *= temp;
        *phi -= *phi / temp;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static bool primes_initialized = false;
    if (!primes_initialized) {
        init_primes();
        primes_initialized = true;
    }

    if (n == 0)          /* not used, but keep defined behaviour */
        return (seq_t)0;

    idx_t found = 0;      /* number of qualifying integers found so far */

    for (idx_t k = 1; ; ++k) {
        uint64_t rad, phi;
        compute_rad_phi((uint64_t)k, &rad, &phi);
        if (rad <= phi + 1ULL) {          /* rad(k) - phi(k) ≤ 1 */
            ++found;
            if (found == n)
                return (seq_t)k;
        }
    }
}
