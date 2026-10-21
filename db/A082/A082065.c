/*
 * A082065  Greatest common prime-divisor of phi(n)=A000010(n) and sigma(2,n)
 *          = A001157(n); a(n) = 1 if no common prime-divisor exists.
 * https://oeis.org/A082065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime divisor of gcd(φ(n), σ₂(n)); if the gcd has
 *            no prime divisor >1, a(n) = 1. φ(n) is Euler's totient function,
 *            σ₂(n) is the sum of squares of the divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* maximum prime needed for factoring: sqrt(max σ₂ for n≤10000) ≈ 1.7e6.
   We generate all primes up to 2,000,000 once. */
#define MAX_PRIME 2000000
static int primes[MAX_PRIME / 10 + 10]; /* enough space for ~150k primes */
static size_t prime_cnt = 0;
static int primes_initialized = 0;

static void init_primes(void) {
    if (primes_initialized) return;
    primes_initialized = 1;
    static unsigned char is_composite[MAX_PRIME + 1];
    size_t i, p, idx = 0;
    is_composite[0] = is_composite[1] = 1;
    for (p = 2; p * p <= MAX_PRIME; ++p) {
        if (!is_composite[p]) {
            for (i = p * p; i <= MAX_PRIME; i += p)
                is_composite[i] = 1;
        }
    }
    for (p = 2; p <= MAX_PRIME; ++p)
        if (!is_composite[p])
            primes[idx++] = (int)p;
    prime_cnt = idx;
}

/* Euclidean gcd for 64‑bit integers. */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Compute Euler's totient φ(n). */
static uint64_t totient_uint64(uint64_t n)
{
    uint64_t phi = n;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            phi = phi / p * (p - 1);
        }
    }
    if (temp > 1) phi = phi / temp * (temp - 1);
    return phi;
}

/* Compute σ₂(n): sum of squares of divisors. */
static uint64_t sigma2_uint64(uint64_t n)
{
    uint64_t sigma = 1;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* term = 1 + p^2 + p^4 + ... + p^{2e} */
            uint64_t term = 1;
            uint64_t curr = 1;
            for (unsigned j = 0; j < e; ++j) {
                curr *= (p * p);
                term += curr;
            }
            sigma *= term;
        }
    }
    if (temp > 1) {                    /* remaining prime factor */
        sigma *= (1ULL + temp * temp);
    }
    return sigma;
}

/* Return the largest prime divisor of x.
   If x ≤ 1, return 1. */
static uint64_t largest_prime_factor(uint64_t x)
{
    if (x <= 1) return 1;
    uint64_t largest = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= x; ++i) {
        uint64_t p = primes[i];
        if (x % p == 0) {
            largest = p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) largest = x;         /* remaining factor is prime */
    return largest;
}

seq_t a(idx_t n)
{
    init_primes();
    uint64_t N = (uint64_t)n;
    uint64_t phi = totient_uint64(N);
    uint64_t sigma2 = sigma2_uint64(N);
    uint64_t g = gcd_uint64(phi, sigma2);
    uint64_t prime = largest_prime_factor(g);
    return (seq_t)prime;
}
