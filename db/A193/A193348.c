/*
 * A193348  Number of odd divisors of tau(n).
 * https://oeis.org/A193348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of odd divisors of τ(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Small prime sieve up to 1 000 000 (enough for n ≤ 10¹²) */
#define PRIME_SIEVE_MAX 1000000
static uint32_t PRIMES[PRIME_SIEVE_MAX];
static size_t PRIME_CNT = 0;
static bool PRIMES_BUILT = false;

static void build_primes(void)
{
    if (PRIMES_BUILT) return;
    bool *is_prime = (bool*)calloc(PRIME_SIEVE_MAX + 1, sizeof(bool));
    if (!is_prime) return; /* @unreachable: memory failure */
    memset(is_prime, true, PRIME_SIEVE_MAX + 1);
    is_prime[0] = is_prime[1] = false;
    for (uint32_t i = 2; i * i <= PRIME_SIEVE_MAX; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= PRIME_SIEVE_MAX; j += i)
                is_prime[j] = false;
    for (uint32_t i = 2; i <= PRIME_SIEVE_MAX; ++i)
        if (is_prime[i]) PRIMES[PRIME_CNT++] = i;
    free(is_prime);
    PRIMES_BUILT = true;
}

/* Count divisors of an odd number (m) */
static uint64_t odd_divisor_count(uint64_t m)
{
    if (m == 1ULL) return 1ULL;
    uint64_t cnt = 1ULL;
    for (size_t i = 0; i < PRIME_CNT && (uint64_t)PRIMES[i] * PRIMES[i] <= m; ++i) {
        uint32_t p = PRIMES[i];
        uint32_t exp = 0;
        while (m % p == 0U) {
            m /= p;
            exp++;
        }
        if (exp) cnt *= (exp + 1ULL);
    }
    if (m > 1ULL) cnt *= 2ULL; /* remaining prime factor */
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure primes are ready */
    build_primes();

    /* Compute τ(n) */
    uint64_t nn = n;
    uint64_t tau = 1ULL;
    for (size_t i = 0; i < PRIME_CNT && (uint64_t)PRIMES[i] * PRIMES[i] <= nn; ++i) {
        uint32_t p = PRIMES[i];
        uint32_t exp = 0;
        while (nn % p == 0U) {
            nn /= p;
            exp++;
        }
        if (exp) tau *= (exp + 1ULL);
    }
    if (nn > 1ULL) tau *= 2ULL; /* remaining prime factor */

    /* Remove the factor of 2 from τ(n) to get its odd part */
    while ((tau & 1ULL) == 0ULL) tau >>= 1;

    /* Count odd divisors of τ(n), i.e., divisors of the odd part */
    uint64_t result = odd_divisor_count(tau);

    return (seq_t)result;
}
