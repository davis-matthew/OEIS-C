/*
 * A157173  Primes with sum of smallest digit and final digit is a prime.
 * https://oeis.org/A157173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime p such that the sum of the smallest *
 *            digit of p and its last digit is a prime number
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Static data ------------------------------------------- */
static seq_t  *cache      = NULL;   /* cache of sequence values          */
static idx_t   cache_len  = 0;      /* number of terms stored           */
static idx_t   cache_cap  = 0;      /* allocated capacity of cache      */

/* primes list for trial division */
static uint64_t *prime_list     = NULL;
static size_t   prime_cap       = 0;
static size_t   prime_cnt       = 0;

/* next odd number to test for primality */
static uint64_t current_candidate = 5;

/* initialisation flag */
static bool initialized = false;

/* ---------- Helper functions ------------------------------------- */
static inline void ensure_prime_capacity(size_t needed)
{
    if (prime_cap < needed) {
        size_t new_cap = prime_cap ? prime_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                            new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

static inline void add_prime(uint64_t p)
{
    ensure_prime_capacity(prime_cnt + 1);
    prime_list[prime_cnt++] = p;
}

static inline bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

static inline bool good_prime(uint64_t p)
{
    /* compute smallest digit and last digit */
    uint64_t last = p % 10;
    uint64_t min_digit = 9;
    uint64_t tmp = p;
    while (tmp) {
        uint64_t d = tmp % 10;
        if (d < min_digit) min_digit = d;
        tmp /= 10;
    }
    uint64_t sum = min_digit + last;
    return (sum == 2 || sum == 3 || sum == 5 || sum == 7 ||
            sum == 11 || sum == 13 || sum == 17);
}

static inline void ensure_cache_capacity(idx_t needed)
{
    if (cache_cap < needed) {
        idx_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ---------- Main function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* initialise prime list with 2 and 3 */
        add_prime(2);
        add_prime(3);
        current_candidate = 5;          /* next candidate to test */
        initialized = true;
    }

    if (n == 0) return (seq_t)0;        /* undefined for n=0 */

    /* make sure the cache can hold n terms */
    if (cache_cap < n) ensure_cache_capacity(n);

    /* generate sequence terms until we have at least n values */
    while (cache_len < n) {
        if (is_prime_uint64(current_candidate)) {
            add_prime(current_candidate);
            if (good_prime(current_candidate))
                cache[cache_len++] = (seq_t)current_candidate;
        }
        current_candidate += 2;           /* only odd candidates > 3 */
    }

    return cache[n - 1];
}
