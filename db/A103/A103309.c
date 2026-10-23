/*
 * A103309  Smallest prime primitive root of n that is less than n, or 0 if
 *          none exists.
 * https://oeis.org/A103309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest prime primitive root of n that is < n, or 0 if
 *            none exists
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ugcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ugcd.h"

/* Prime sieve up to 1 000 000 – enough for n up to ≈10⁶ (our limits) */
#define PRIME_SIEVE_LIMIT 1000000
#define MAX_PRIMES 80000   /* more than needed for the limit above */

static bool primes_initialized = false;
static uint32_t primes[MAX_PRIMES];
static size_t prime_cnt = 0;
static bool is_composite[PRIME_SIEVE_LIMIT + 1];

/* initialise prime table once */
static void init_primes(void)
{
    if (primes_initialized) return;
    primes_initialized = true;
    prime_cnt = 0;
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
}

/* gcd for 64‑bit */

/* modular exponentiation – uses __int128 for safety */
static uint64_t modpow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1 % mod;
    uint64_t base = a % mod;
    while (e) {
        if (e & 1U) res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        e >>= 1;
    }
    return res;
}

/* check whether n > 1 is a power of a single odd prime */
static bool is_power_of_prime(uint64_t n)
{
    if (n < 2) return false;
    uint64_t p = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t pr = primes[i];
        if ((uint64_t)pr * pr > n) break;
        if (n % pr == 0) {
            p = pr;
            break;
        }
    }
    if (p == 0) return true;           /* n is prime itself */
    while (n % p == 0) n /= p;
    return n == 1;
}

/* compute Euler's totient of n */
static uint64_t compute_phi(uint64_t n)
{
    uint64_t result = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) {
            result = result / p * (p - 1);
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) result = result / n * (n - 1);
    return result;
}

/* main function required by the build */
seq_t a(idx_t n)
{
    if (n <= 2u) return 0;
    init_primes();

    uint64_t N = (uint64_t)n;

    /* special case n == 4 */
    if (N == 4u) return 3;

    /* check existence of primitive roots: n must be p^k or 2*p^k with p odd prime */
    if ((N & 1u) == 0) {          /* even n */
        if ((N & 3u) == 0)        /* divisible by 4 but not equal to 4 */
            return 0;
        uint64_t odd_part = N >> 1;
        if (!is_power_of_prime(odd_part)) return 0;
    } else {                      /* odd n */
        if (!is_power_of_prime(N)) return 0;
    }

    /* compute φ(n) and its distinct prime factors */
    uint64_t phi = compute_phi(N);
    uint64_t phi_tmp = phi;
    uint64_t phi_factors[32];
    size_t phi_fc = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= phi_tmp; ++i) {
        uint32_t p = primes[i];
        if (phi_tmp % p == 0) {
            phi_factors[phi_fc++] = p;
            while (phi_tmp % p == 0) phi_tmp /= p;
        }
    }
    if (phi_tmp > 1) phi_factors[phi_fc++] = phi_tmp;

    /* find the smallest prime < n that is primitive */
    for (size_t i = 0; i < prime_cnt && primes[i] < N; ++i) {
        uint32_t g = primes[i];
        if (ugcd((uint64_t)g, N) != 1) continue;   /* not coprime */
        bool primitive = true;
        for (size_t j = 0; j < phi_fc; ++j) {
            uint64_t exp = phi / phi_factors[j];
            if (modpow_uint64(g, exp, N) == 1) {
                primitive = false;
                break;
            }
        }
        if (primitive) return (seq_t)g;
    }
    return 0;
}
