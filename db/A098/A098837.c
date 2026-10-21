/*
 * A098837  Smith semiprimes.
 * https://oeis.org/A098837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 886, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098837: Smith semiprimes
   a(n) = the n-th positive integer that is both a Smith number
          (sum of its digits equals sum of digits of its prime factors,
           counted with multiplicity) and a semiprime (product of two
           primes, duplicates allowed).  The function scans integers,
           tests for semiprimality and the Smith property, and memoizes
           the values for fast repeated access.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --- Helper: digit sum for unsigned integers --------------------------------- */
static inline uint64_t digit_sum_uint64(uint64_t x)
{
    uint64_t s = 0;
    do {
        s += x % 10;
        x /= 10;
    } while (x);
    return s;
}

/* --- Prime generation ------------------------------------------------------- */
#define PRIME_ARRAY_CAP 400000          /* enough for primes <= 5 000 000   */
static uint32_t primes[PRIME_ARRAY_CAP];
static size_t prime_cnt = 0;

/* Generate all primes up to LIMIT using a simple sieve */
static void init_primes(void)
{
    if (prime_cnt > 0) return;          /* already initialized */

    const uint32_t LIMIT = 5000000;      /* sqrt of the largest candidate we expect */
    uint8_t *sieve = (uint8_t*)malloc(LIMIT + 1);
    if (!sieve) return;                 /* allocation failure (unlikely) */
    memset(sieve, 0, LIMIT + 1);

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) {
            if (prime_cnt < PRIME_ARRAY_CAP)
                primes[prime_cnt++] = i;
            if (i * (uint64_t)i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                    sieve[(size_t)j] = 1;
            }
        }
    }
    free(sieve);
}

/* --- Test whether a 64‑bit integer is a Smith semiprime --------------------- */
static bool is_smith_semiprime(uint64_t n)
{
    if (n <= 3) return false;          /* semiprime must be at least 4 */

    uint64_t m = n;
    uint64_t sum_digits_n = digit_sum_uint64(m);
    uint64_t sum_digits_factors = 0;
    int total_factors = 0;

    /* Factor using precomputed primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > m) break;
        if (m % p == 0) {
            while (m % p == 0) {
                m /= p;
                sum_digits_factors += digit_sum_uint64(p);
                ++total_factors;
                if (total_factors > 2) return false;   /* too many prime factors */
            }
        }
    }

    /* Remaining prime factor (if any) */
    if (m > 1) {
        sum_digits_factors += digit_sum_uint64(m);
        ++total_factors;
        if (total_factors > 2) return false;
    }

    return (total_factors == 2) && (sum_digits_factors == sum_digits_n);
}

/* --- Memoised sequence construction ---------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 4;           /* first possible Smith semiprime */

/* Helper: append a value to the cache, expanding as needed */
static void cache_append(uint64_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 64;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = (seq_t)val;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    init_primes();                         /* ensure primes are ready */

    if (n == 0) return (seq_t)0;           /* undefined but safe */

    /* Ensure cache contains at least n terms */
    while (cache_len < (size_t)n) {
        while (!is_smith_semiprime(next_candidate))
            ++next_candidate;
        cache_append(next_candidate);
        ++next_candidate;
    }
    return cache[n - 1];
}
