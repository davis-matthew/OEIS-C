/*
 * A103496  Multiplicative suborder of 10 (mod 2n+1) = sord(10, 2n+1).
 * https://oeis.org/A103496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = minimum e ≥ 1 such that 10^e ≡ ±1 (mod 2n+1), or 0 if no
 *            such e exists. Offset = 0
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Small primes up to 151 are enough for factoring numbers ≤ 20001. */
static const uint32_t primes_list[] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
    31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u,
    71u, 73u, 79u, 83u, 89u, 97u, 101u, 103u, 107u,
    109u, 113u, 127u, 131u, 137u, 139u, 149u, 151u
};
static const size_t prime_count = sizeof(primes_list) / sizeof(primes_list[0]);

/* Compute Euler's totient φ(m) for m ≤ 20001. */
static uint64_t compute_phi_uint64(uint64_t m) {
    uint64_t result = m;
    uint64_t t = m;
    for (size_t i = 0; i < prime_count && (uint64_t)primes_list[i] * (uint64_t)primes_list[i] <= t; ++i) {
        uint32_t p = primes_list[i];
        if (t % p == 0) {
            while (t % p == 0)
                t /= p;
            result -= result / p;
        }
    }
    if (t > 1)
        result -= result / t;
    return result;
}

/* Modular exponentiation: base^exp mod mod, all < 2^64. */
static seq_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t r = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1ULL)
            r = (r * b) % mod;
        b = (b * b) % mod;
        exp >>= 1ULL;
    }
    return (seq_t)r;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;  /* a(0) = 0 by definition */

    uint64_t m = 2ULL * (uint64_t)n + 1ULL;   /* modulus 2n+1 */

    /* If 5 divides m, 10 is not invertible modulo m, so no e>0 gives ±1. */
    if (m % 5ULL == 0ULL)
        return 0;

    /* φ(m) gives an upper bound on the multiplicative order. */
    uint64_t phi_val = compute_phi_uint64(m);

    /* Factor φ(m).  Store prime factors and their exponents. */
    uint64_t fact_primes[32];
    uint32_t fact_exp[32];
    size_t fact_cnt = 0;
    uint64_t tmp = phi_val;
    for (size_t i = 0; i < prime_count && (uint64_t)primes_list[i] * (uint64_t)primes_list[i] <= tmp; ++i) {
        uint32_t p = primes_list[i];
        if (tmp % p == 0) {
            fact_primes[fact_cnt] = p;
            fact_exp[fact_cnt] = 0;
            do {
                tmp /= p;
                fact_exp[fact_cnt] += 1;
            } while (tmp % p == 0);
            fact_cnt += 1;
        }
    }
    if (tmp > 1) {
        fact_primes[fact_cnt] = tmp;
        fact_exp[fact_cnt] = 1;
        fact_cnt += 1;
    }

    /* Generate all divisors of φ(m) into a stack array. */
    uint64_t divs[256];
    size_t divcnt = 1;           /* always at least 1 (divisor 1) */
    divs[0] = 1ULL;

    for (size_t i = 0; i < fact_cnt; ++i) {
        uint64_t p = fact_primes[i];
        uint32_t e = fact_exp[i];
        size_t curcnt = divcnt;
        uint64_t pow = 1ULL;
        for (uint32_t k = 1; k <= e; ++k) {
            pow *= p;
            for (size_t j = 0; j < curcnt; ++j) {
                if (divcnt < 256)  /* defensive; should never overflow */
                    divs[divcnt++] = divs[j] * pow;
            }
        }
    }

    /* Find the minimal divisor e giving 10^e ≡ ±1 (mod m). */
    uint64_t min_e = 0;  /* 0 means not found yet */
    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t e = divs[i];
        seq_t r = modpow_uint64(10ULL, e, m);
        uint64_t rv = (uint64_t)r;  /* safe, rv < m */
        if (rv == 1ULL || rv == (m - 1ULL)) {
            if (min_e == 0 || e < min_e)
                min_e = e;
        }
    }

    return (seq_t)min_e;  /* 0 if no such e, otherwise minimal e */
}
