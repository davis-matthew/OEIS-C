/*
 * A070817  a(n) = floor(n/2) - gpf(phi(n)), where gpf(n) is the largest prime
 *          factor of n.
 * https://oeis.org/A070817
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = floor(n/2) - gpf(phi(n))
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_PRIME 200000
/* enough primes for factoring n and φ(n) when n ≤ 10000 */
static uint32_t primes[MAX_PRIME / 10];
static size_t prime_cnt = 0;
static bool sieve_inited = false;

/* Build a small prime sieve once */
static void init_sieve(void)
{
    if (sieve_inited) return;
    static unsigned char is_comp[MAX_PRIME + 1] = {0};
    for (uint32_t i = 2; i * i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= MAX_PRIME; j += i)
                is_comp[j] = 1;
        }
    }
    for (uint32_t i = 2; i <= MAX_PRIME; ++i)
        if (!is_comp[i]) primes[prime_cnt++] = i;
    sieve_inited = true;
}

/* Euler's totient function for n (idx_t) */
static uint64_t totient(idx_t n)
{
    if (n <= 1) return 1;
    uint64_t result = (uint64_t)n;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            result -= result / p;
        }
    }
    if (tmp > 1) result -= result / tmp;
    return result;
}

/* Largest prime factor of v (v ≥ 0) */
static uint64_t largest_prime_factor(uint64_t v)
{
    if (v <= 1) return 0;
    uint64_t gpf = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= v; ++i) {
        uint32_t p = primes[i];
        if (v % p == 0) {
            gpf = p;
            while (v % p == 0) v /= p;
        }
    }
    if (v > 1) gpf = v; /* remaining v is prime */
    return gpf;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_sieve();

    /* floor(n/2) */
    seq_t half = (seq_t)(n >> 1);

    /* φ(n) */
    uint64_t phi_val = totient(n);

    /* largest prime factor of φ(n) */
    uint64_t gpf_val = largest_prime_factor(phi_val);

    return half - (seq_t)gpf_val;
}
