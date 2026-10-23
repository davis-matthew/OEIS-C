/*
 * A134020  Numbers that are one less than a square and have exactly 4
 *          divisors.
 * https://oeis.org/A134020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers one less than a square that have exactly 4
 *            divisors (computed by scanning successive squares and testing
 *            for 4 divisors)
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 861, u64 n <= 5400,
 *            u128 n <= 5400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* ---------- Prime sieve (up to a fixed large limit) ---------- */
static uint32_t *prime_list = NULL;
static size_t      prime_cnt  = 0;

static void init_primes(void) {
    if (prime_list) return; /* already initialized */

    const size_t LIMIT = 5000000;    /* enough for n up to at least 10000 */
    bool *is_prime = (bool *)malloc(LIMIT + 1);
    if (!is_prime) exit(EXIT_FAILURE);

    for (size_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;

    size_t sqrtL = (size_t)floor(sqrt((double)LIMIT)) + 1;
    for (size_t i = 2; i <= sqrtL; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) prime_list[idx++] = (uint32_t)i;
    }
    prime_cnt = cnt;
    free(is_prime);
}

/* ---------- Test whether m has exactly 4 divisors ---------- */
static bool has4div(uint64_t n) {
    if (n < 2) return false;          /* 1 has 1 divisor */

    uint64_t temp = n;
    size_t   num_factors = 0;
    uint32_t exp1 = 0, exp2 = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > temp) break;

        if (temp % p == 0) {
            uint32_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            ++num_factors;
            if (num_factors == 1) exp1 = exp;
            else if (num_factors == 2) exp2 = exp;
            else return false;             /* more than two prime factors */
        }
    }

    if (temp > 1) {
        /* remaining prime factor */
        ++num_factors;
        if (num_factors == 1) exp1 = 1;
        else if (num_factors == 2) exp2 = 1;
        else return false;
    }

    /* Now check the only possibilities for exactly 4 divisors */
    if (num_factors == 1 && exp1 == 3) return true;            /* p^3 */
    if (num_factors == 2 && exp1 == 1 && exp2 == 1) return true; /* p*q */
    return false;
}

/* ---------- Sequence cache ---------- */
static seq_t *cache       = NULL;
static size_t cache_size  = 0;
static size_t cache_cap   = 0;
static idx_t  last_k      = 2;   /* next square root to test */

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;             /* undefined, but harmless */

    /* Ensure primes are ready */
    if (!prime_list) init_primes();

    /* Grow cache until we have the nth term */
    while (cache_size < (size_t)n) {
        /* Scan squares starting from last_k */
        while (true) {
            idx_t k = last_k++;
            uint64_t m64 = (uint64_t)k * (uint64_t)k - 1ULL;

            if (has4div(m64)) {
                seq_t val = (seq_t)k * (seq_t)k - 1; /* fits in seq_t */

                /* grow cache if needed */
                if (cache_size == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!new_arr) exit(EXIT_FAILURE);
                    cache = new_arr;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = val;
                break;  /* found the next term */
            }
        }
    }

    return cache[n - 1];   /* 1‑based indexing */
}
