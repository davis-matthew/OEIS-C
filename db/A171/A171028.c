/*
 * A171028  In the sequence of prime numbers, replace all the '6' digits with
 *          '0' and vice versa.
 * https://oeis.org/A171028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = swap_digits(p_n) where p_n is the n‑th prime
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- Prime cache ----------------------------------------------------- */

static uint64_t *prime_cache = NULL;   /* stored primes  (uint64_t works up to 1e5) */
static size_t   cache_cnt   = 0;       /* number of primes cached */
static size_t   cache_cap   = 0;       /* allocated capacity */
static uint64_t last_checked = 1;      /* last tested number */

/* Grow the prime cache when needed */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    void *tmp = realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);           /* abort on allocation failure */
    prime_cache = (uint64_t*)tmp;            /* cast for C++ */
    cache_cap   = new_cap;
}

/* Test whether x is prime using the current cache of primes */
static bool is_prime_candidate(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < cache_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Generate primes until the cache has at least n primes */
static void grow_primes_to(size_t n)
{
    if (cache_cnt >= n) return;
    while (cache_cnt < n) {
        ++last_checked;
        if (is_prime_candidate(last_checked)) {
            ensure_capacity(cache_cnt + 1);
            prime_cache[cache_cnt++] = last_checked;
        }
    }
}

/* --- Digit manipulation ---------------------------------------------- */

/* Replace digits 6↔0 in a number (no leading zero preservation) */
static seq_t swap_digits(seq_t n)
{
    if (n == 0) return 0;
    seq_t result = 0;
    seq_t mult   = 1;
    while (n != 0) {
        seq_t d = n % 10;
        n /= 10;
        if (d == 6) d = 0;
        else if (d == 0) d = 6;
        result += d * mult;
        mult   *= 10;
    }
    return result;
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* undefined for n < 1 */
    grow_primes_to((size_t)n);                 /* ensure we have n primes */
    seq_t p = (seq_t)prime_cache[n - 1];        /* nth prime (1‑based) */
    return swap_digits(p);
}
