/*
 * A085710  Smallest k such that kn+1 is a semiprime.
 * https://oeis.org/A085710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest k such that n*k + 1 is a semiprime */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* small prime sieve up to LIMIT */
#define PRIME_LIMIT 100000

static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static bool primes_ready = false;

/* initialize the prime list once */
static void init_primes(void)
{
    if (primes_ready) return;

    bool *is_prime = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    memset(is_prime, 1, (PRIME_LIMIT + 1) * sizeof(bool));
    is_prime[0] = is_prime[1] = 0;

    for (uint32_t i = 2; i * (uint32_t)i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;

    /* count primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = i;

    free(is_prime);
    prime_count = cnt;
    primes_ready = true;
}

/* check if x is a semiprime (exactly two primes, multiplicity counted) */
static bool is_semiprime(uint64_t x)
{
    init_primes();

    int cnt = 0;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= x; ++i) {
        uint32_t p = prime_list[i];
        while (x % p == 0) {
            ++cnt;
            if (cnt > 2) return false;
            x /= p;
        }
    }
    if (x > 1) ++cnt;
    return cnt == 2;
}

/* main sequence function (external linkage) */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;   /* by definition */
    uint64_t nn = (uint64_t)n;

    for (uint64_t k = 1; ; ++k) {
        uint64_t val = nn * k + 1;
        if (is_semiprime(val))
            return (seq_t)k;
    }
}
