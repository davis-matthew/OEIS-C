/*
 * A082063  Greatest common prime divisor of n and sigma_2(n) = A001157(n), or
 *          1 if the two are relatively prime.
 * https://oeis.org/A082063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest common prime divisor of n and sigma_2(n), or 1
 *            if the two are coprime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ----- Static sieve for trial division ----- */
static int primes[100000];
static int prime_cnt = 0;
static int primes_initialized = 0;

static void init_primes(void) {
    int limit = 100000;
    static char sieve[100001];
    for (int i = 2; i <= limit; ++i) sieve[i] = 1;
    for (int i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= limit; j += i) sieve[j] = 0;
        }
    }
    for (int i = 2; i <= limit; ++i) {
        if (sieve[i]) primes[prime_cnt++] = i;
    }
    primes_initialized = 1;
}

/* ----- GCD for 64‑bit values ----- */
static uint64_t ugcd(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ----- Sigma_2 (sum of squares of divisors) for 64‑bit values ----- */
static uint64_t sigma2_value(uint64_t n) {
    if (!primes_initialized) init_primes();
    uint64_t result = 1;
    uint64_t orig = n;
    for (int i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= n; ++i) {
        int p = primes[i];
        if (n % p == 0) {
            uint64_t sum = 1;            /* accounts for exponent 0 */
            __int128 term = 1;           /* current p^(2k) as __int128 to avoid overflow */
            while (n % p == 0) {
                n /= p;
                term = term * p * p;      /* next p^(2k) */
                sum += (uint64_t)term;    /* add to the sum of squares for this prime power */
            }
            result *= sum;
        }
    }
    if (n > 1) {                       /* remaining prime factor > sqrt(orig) */
        result *= (1 + n * n);
    }
    return result;
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n) {
    uint64_t u = (uint64_t)n;
    /* 1. Compute sigma_2(n) */
    uint64_t s2 = sigma2_value(u);

    /* 2. Compute gcd(n, sigma_2(n)) */
    uint64_t g = ugcd(u, s2);
    if (g == 1) return 1;

    /* 3. Find the largest prime divisor of g */
    if (!primes_initialized) init_primes();
    uint64_t temp = g;
    uint64_t maxp = 1;

    for (int i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        uint64_t pu = (uint64_t)p;
        if (pu * pu > temp) break;
        if (temp % p == 0) {
            maxp = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {            /* remaining prime factor */
        if (temp > maxp) maxp = temp;
    }

    return (seq_t)maxp;
}
