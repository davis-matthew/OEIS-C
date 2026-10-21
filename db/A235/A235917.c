/*
 * A235917  a(n) = |{0 < k < n - 2: p = prime(k) + phi(n-k)/2, p^2 - 1 -
 *          prime(p) and (p^2 - 1)/2 - prime(p) are all prime}|, where phi(.)
 *          is Euler's totient function.
 * https://oeis.org/A235917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts 0 < k < n-2 such that p = prime(k) + phi(n-k)/2 is
 *            prime, prime(p) is the p‑th prime, and p²-1-prime(p) and
 *            (p²-1)/2-prime(p) are prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, mod_mul, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* utilities supplied by the build system */
#include "../utils/build_primes_upto.h"   /* build_primes_upto(limit,&primes,&cnt) */
#include "../utils/mod_mul.h"             /* mod_mul(uint64_t a,b,mod) */
#include "../utils/powmod64.h"            /* powmod64(base,exp,mod) */

static uint64_t *primes = NULL;      /* primes[i] is the (i+1)-th prime */
static size_t prime_cnt = 0;
static bool primes_ready = false;

/* initialise the prime table (up to 2 000 000 – enough for n≈10 000) */
static void init_primes(void)
{
    if (primes_ready) return;
    const idx_t LIMIT = 2000000;
    build_primes_upto(LIMIT, &primes, &prime_cnt);
    primes_ready = true;
}

/* Euler’s totient for 64‑bit values using the stored primes */
static uint64_t phi_u64(uint64_t m)
{
    uint64_t result = m;
    uint64_t temp = m;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result = result / p * (p - 1);
        }
    }
    if (temp > 1) {
        result = result / temp * (temp - 1);
    }
    return result;
}

/* deterministic Miller–Rabin primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t sp = small_primes[i];
        if (n == sp) return true;
        if (n % sp == 0) return false;
    }
    /* write n-1 as d * 2^s */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }
    /* bases sufficient for n < 3·10^12, our values are far below */
    const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a >= n) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_primes();

    if (n < 3) return 0;

    uint64_t count = 0;
    for (idx_t k = 1; k <= n - 3; ++k) {
        uint64_t pk = primes[(size_t)k - 1];
        uint64_t phi_val = phi_u64((uint64_t)(n - k));

        uint64_t p = pk + phi_val / 2;
        if (!is_prime_uint64(p)) continue;
        if (p == 0 || p > prime_cnt) continue;        /* safety */

        uint64_t prime_p = primes[p - 1];

        uint64_t p2 = p * p;
        uint64_t v1 = p2 - 1 - prime_p;
        if (!is_prime_uint64(v1)) continue;

        uint64_t v2 = (p2 - 1) / 2 - prime_p;
        if (!is_prime_uint64(v2)) continue;

        ++count;
    }

    return (seq_t)count;
}
