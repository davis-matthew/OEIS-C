/*
 * A088873  Number of different values of A000005(d) where d is a divisor of
 *          n.
 * https://oeis.org/A088873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_tau
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of distinct values of d(d) for all divisors d of n,
 * where d(x) is the number of positive divisors of x (OEIS A000005).
 * The implementation factors n, recursively enumerates all divisor exponent
 * vectors, computes the divisor-count of each divisor, and records the
 * distinct values in a boolean array.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/dfs_tau.h"

/* ---------- local utility: sieve of primes up to 1e6 ---------- */
static int sieve_done = 0;
static uint32_t primes[78498];   /* primes <= 1e6 */
static size_t prime_cnt = 0;

static void init_sieve(void)
{
    if (sieve_done) return;
    const uint32_t N = 1000000;
    uint8_t *is_prime = (uint8_t *)malloc(N + 1);
    memset(is_prime, 1, N + 1);
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; i * i <= N; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= N; j += i)
                is_prime[j] = 0;
        }
    }
    size_t idx = 0;
    for (uint32_t i = 2; i <= N; ++i)
        if (is_prime[i]) primes[idx++] = i;
    prime_cnt = idx;
    free(is_prime);
    sieve_done = 1;
}

/* ---------- local utility: factorise n into prime factors ---------- */
static void local_factorise(uint64_t n, uint64_t *p, uint64_t *e, size_t *cnt)
{
    init_sieve();
    size_t c = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= n; ++i) {
        uint32_t pr = primes[i];
        if (n % pr == 0) {
            uint64_t exp = 0;
            do {
                n /= pr;
                ++exp;
            } while (n % pr == 0);
            p[c] = pr;
            e[c] = exp;
            ++c;
        }
    }
    if (n > 1) {          /* remaining prime factor */
        p[c] = n;
        e[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* ---------- required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined; safety */
    uint64_t primes_arr[64];
    uint64_t exponents[64];
    size_t factor_cnt;
    local_factorise((uint64_t)n, primes_arr, exponents, &factor_cnt);

    /* maximum possible τ value is the number of divisors of n */
    uint64_t max_tau = 1;
    for (size_t i = 0; i < factor_cnt; ++i)
        max_tau *= (exponents[i] + 1);

    /* visited array for τ values */
    bool *seen = (bool *)calloc((size_t)(max_tau + 1), sizeof(bool));
    if (seen == NULL) return 0; /* allocation failure fallback */

    int distinct = 0;
    dfs_tau(exponents, factor_cnt, 0, 1, seen, &distinct);

    free(seen);
    return (seq_t)distinct;
}
