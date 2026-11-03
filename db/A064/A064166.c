/*
 * A064166  Composite numbers the sum of the digits in the prime factors of
 *          which, with repetition, is prime.
 * https://oeis.org/A064166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 979 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A064166
 *  Composite numbers for which the sum of the digits of
 *  the prime factors (with multiplicity) is itself a prime.
 *
 *  The function generates the sequence by testing successive
 *  composite numbers, factorising them with a small prime sieve,
 *  summing the decimal digits of each prime factor, and checking
 *  that the sum is prime.
 *
 *  It maintains a static cache so that calls with arbitrary
 *  indices are fast and independent of order.  The implementation
 *  works for any seq_t / idx_t defined by the build and uses only
 *  the allowed operations and helper utilities.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* --- Prime sieve ----------------------------------------------------- */
static const size_t SIEVE_LIMIT = 2000000;
static bool *is_prime_bool = NULL;          /* flag array          */
static uint32_t *prime_list  = NULL;        /* primes up to limit  */
static size_t  prime_cnt    = 0;            /* number of primes    */

static void init_sieve(void)
{
    if (is_prime_bool != NULL) return;   /* already initialized */

    /* Memory for sieve */
    is_prime_bool = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_prime_bool) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime_bool[i] = true;
    is_prime_bool[0] = is_prime_bool[1] = false;

    /* Eratosthenes */
    for (size_t p = 2; p * p <= SIEVE_LIMIT; ++p) {
        if (is_prime_bool[p]) {
            for (size_t q = p * p; q <= SIEVE_LIMIT; q += p)
                is_prime_bool[q] = false;
        }
    }

    /* Store primes in array */
    size_t cap = SIEVE_LIMIT / 10 + 1;     /* generous capacity   */
    prime_list = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);
    prime_cnt = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime_bool[i]) prime_list[prime_cnt++] = (uint32_t)i;
}

/* --- Cached sequence ---------------------------------------------- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t next_candidate = 4; /* start testing from 4 */

seq_t a(idx_t n)
{
    init_sieve(); /* ensure sieve ready */

    /* Ensure cache has enough space for index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we reach index n */
    while (cache_len <= (size_t)n) {
        uint64_t m = next_candidate++;

        /* Skip primes; only composites are candidates */
        bool m_is_prime;
        if (m <= SIEVE_LIMIT)
            m_is_prime = is_prime_bool[(size_t)m];
        else
            m_is_prime = is_prime_big((seq_t)m);
        if (m_is_prime) continue;

        /* Factor m and compute sum of decimal digits of prime factors */
        uint64_t temp = m;
        uint64_t sum_digits = 0;

        for (size_t i = 0; i < prime_cnt &&
             (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= temp; ++i) {
            uint32_t p = prime_list[i];
            while (temp % p == 0) {
                temp /= p;
                sum_digits += (uint64_t)digit_sum((seq_t)p);
            }
        }
        if (temp > 1) { /* remaining prime factor > sqrt(m) */
            sum_digits += (uint64_t)digit_sum((seq_t)temp);
        }

        /* Check if the digit sum is prime */
        if (is_prime_big((seq_t)sum_digits)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
    }

    return cache[(size_t)n];
}
