/*
 * A064449  a(n) = gcd(phi(n!), phi(n^n), phi(lcm(1..n))).
 * https://oeis.org/A064449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(phi(n!), phi(n^n), phi(lcm(1..n)))
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 29, u64 n <= 83, u128 n <= 149,
 *            big any n
 * Helpers:   gcd, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

/* external helper functions */
#include "../utils/gcd.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* global prime tables */
#define PRIME_LIMIT 10000
#define MAX_PRIME_CNT 2000     /* enough for primes <= 10000 */

static bool primes_initialized = false;
static size_t prime_cnt = 0;
static uint32_t primes_u32[MAX_PRIME_CNT];
static uint64_t primes_u64[MAX_PRIME_CNT];
static int     prime_idx_map[PRIME_LIMIT + 1];

/* initialize prime tables once */
static void init_primes(void) {
    if (primes_initialized) return;

    bool sieve[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i) sieve[j] = false;
        }
    }
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) prime_idx_map[i] = -1;

    prime_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) if (sieve[i]) {
        primes_u32[prime_cnt] = (uint32_t)i;
        primes_u64[prime_cnt] = (uint64_t)i;
        prime_idx_map[i] = (int)prime_cnt;
        ++prime_cnt;
    }
    primes_initialized = true;
}

/* add prime factorization of val into exp array (array indices correspond to primes list) */
static void factor_add(uint64_t val, uint64_t exp[], size_t plc) {
    if (val <= 1) return;
    for (size_t i = 0; i < plc && (uint64_t)primes_u32[i] * primes_u32[i] <= val; ++i) {
        uint32_t p = primes_u32[i];
        if (val % p == 0) {
            uint64_t c = 0;
            while (val % p == 0) {
                val /= p;
                ++c;
            }
            exp[i] += c;
        }
    }
    if (val > 1) {
        int idx = prime_idx_map[(int)val];
        if (idx >= 0) exp[idx] += 1;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_primes();

    /* exponent arrays for the three phi values */
    static uint64_t e1[MAX_PRIME_CNT];
    static uint64_t e2[MAX_PRIME_CNT];
    static uint64_t e3[MAX_PRIME_CNT];
    static uint64_t n_exp[MAX_PRIME_CNT];
    size_t pc = prime_cnt;

    for (size_t i = 0; i < pc; ++i) {
        e1[i] = e2[i] = e3[i] = 0;
        n_exp[i] = 0;
    }

    /* factor n for later use */
    uint64_t tmp_n = (uint64_t)n;
    for (size_t i = 0; i < pc && (uint64_t)primes_u32[i] * primes_u32[i] <= tmp_n; ++i) {
        uint32_t p = primes_u32[i];
        if (tmp_n % p == 0) {
            uint64_t c = 0;
            while (tmp_n % p == 0) {
                tmp_n /= p;
                ++c;
            }
            n_exp[i] = c;
        }
    }
    if (tmp_n > 1) {
        int idx = prime_idx_map[(int)tmp_n];
        if (idx >= 0) n_exp[idx] = 1;
    }

    /* Compute phi(n!) exponents */
    for (size_t i = 0; i < pc; ++i) {
        uint32_t p = primes_u32[i];
        if (p > (uint32_t)n) break;
        /* exponent of p in n! */
        uint64_t div = n / p;
        uint64_t sum = 0;
        while (div) {
            sum += div;
            div /= p;
        }
        if (sum > 0) e1[i] += sum - 1;          /* p^(sum-1) factor */
        if (p > 2) factor_add((uint64_t)p - 1, e1, pc); /* (p-1) factor */
    }

    /* Compute phi(lcm(1..n)) exponents */
    for (size_t i = 0; i < pc; ++i) {
        uint32_t p = primes_u32[i];
        if (p > (uint32_t)n) break;
        /* exponent of p in lcm */
        uint64_t k = 0;
        uint64_t mult = 1;
        while (mult <= n / p) {
            mult *= p;
            ++k;
        }
        if (k > 0) e3[i] += k - 1;             /* p^(k-1) factor */
        if (p > 2) factor_add((uint64_t)p - 1, e3, pc);
    }

    /* Compute phi(n^n) exponents */
    for (size_t i = 0; i < pc; ++i) {
        if (n_exp[i] == 0) continue;
        uint64_t e = n_exp[i];
        e2[i] += e * (uint64_t)n - 1;         /* p^(e*n-1) factor */
        if (primes_u32[i] > 2) factor_add((uint64_t)primes_u32[i] - 1, e2, pc);
    }

    /* GCD exponent and compute result */
    seq_t result = 1;
    for (size_t i = 0; i < pc; ++i) {
        uint64_t min_e = e1[i];
        if (e2[i] < min_e) min_e = e2[i];
        if (e3[i] < min_e) min_e = e3[i];
        if (min_e == 0) continue;
        seq_t p_pow = seq_pow((seq_t)primes_u32[i], min_e);
        result = seq_mul(result, p_pow);
    }
    return result;
}
