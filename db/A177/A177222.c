/*
 * A177222  Numbers k that are the products of two distinct primes, such that
 *          2*k + 1 and 4*k + 3 are also products of two distinct primes.
 * https://oeis.org/A177222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the nth number k that is a product of two distinct primes
 *             such that 2*k+1 and 4*k+3 are also products of two distinct primes.
 * The function searches positive integers k, storing the found values in a
 * static cache so that calls can be made in any order.  The algorithm
 * tests k for being a semiprime of distinct primes and then checks the
 * two derived numbers in the same way.  All arithmetic is performed using
 * 64‑bit unsigned integers, which is sufficient for the known terms up to
 * n=10 000.  The helper function isPrimeTrial is used for primality
 * testing of integers that exceed the sieve limit.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

#define PRIME_LIMIT 10000000U          /* primes up to 10 million */

static uint8_t *prime_sieve = NULL;   /* bit‑vector: 0 = prime */
static uint32_t *prime_list  = NULL;   /* array of primes */
static size_t   prime_cnt   = 0;        /* number of primes present */

/* ------------------------------------------------------------------ */
/* Build a simple sieve of Eratosthenes up to PRIME_LIMIT.            */
/* ------------------------------------------------------------------ */
static void init_primes(void)
{
    if (prime_sieve != NULL) return;   /* already done */

    prime_sieve = (uint8_t*)calloc((size_t)PRIME_LIMIT + 1, 1);
    if (!prime_sieve) return;          /* allocation failure */

    /* initial capacity for prime list: roughly PRIME_LIMIT / 10 */
    prime_list = (uint32_t*)malloc((size_t)PRIME_LIMIT / 10 * sizeof(uint32_t));
    if (!prime_list) { free(prime_sieve); return; }

    prime_cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (prime_sieve[i] == 0) {          /* i is prime */
            if (prime_cnt % 512 == 0 && prime_cnt != 0) {   /* grow */
                void *tmp = realloc(prime_list,
                                    ((prime_cnt + 512) * sizeof(uint32_t)));
                if (!tmp) break;
                prime_list = (uint32_t*)tmp;
            }
            prime_list[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= PRIME_LIMIT; j += i)
                    prime_sieve[(size_t)j] = 1;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Quick primality test for 64‑bit numbers.                             */
/* Uses the sieve for numbers up to PRIME_LIMIT, otherwise uses the     */
/* generic Miller‑Rabin helper isPrimeTrial().                         */
/* ------------------------------------------------------------------ */
static bool is_prime(uint64_t n)
{
    if (n <= PRIME_LIMIT) return prime_sieve[(size_t)n] == 0;
    return isPrimeTrial((seq_t)n);
}

/* ------------------------------------------------------------------ */
/* Test whether n is a product of two distinct primes.                 */
/* ------------------------------------------------------------------ */
static bool is_semiprime_distinct(uint64_t n)
{
    if (n < 6) return false;   /* smallest product of two distinct primes is 2*3 */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > n) break;
        if (n % p == 0) {
            uint64_t q = n / p;
            return (q != p) && is_prime(q);
        }
    }
    return false;  /* no divisor found: n is prime or has >2 factors */
}

/* ------------------------------------------------------------------ */
/* Cache for generated sequence values.                                 */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;               /* allocation failure unlikely */
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Generate sequence terms up to index n (1‑based).                    */
/* ------------------------------------------------------------------ */
static void generate_up_to(idx_t n)
{
    if ((size_t)n <= cache_len) return;   /* already have enough terms */

    init_primes();                      /* ensure primes are ready */

    uint64_t start_k = 2;
    if (cache_len > 0)
        start_k = (uint64_t)cache[cache_len - 1] + 1;

    for (uint64_t k = start_k; cache_len < (size_t)n; ++k) {
        if (!is_semiprime_distinct(k)) continue;
        uint64_t m1 = 2 * k + 1;
        uint64_t m2 = 4 * k + 3;
        if (is_semiprime_distinct(m1) && is_semiprime_distinct(m2)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function.                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset */
    generate_up_to(n);
    return cache[n - 1];
}
