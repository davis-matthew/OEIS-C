/*
 * A191026  Primes p that have Jacobi symbol (p|35) = 1.
 * https://oeis.org/A191026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   good_jacobi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191026: the n-th prime p such that Jacobi symbol (p|35)=1 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/good_jacobi.h"

/* Prime list for small prime tests */
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

/* Cache of qualifying primes (type seq_t, arbitrary‑precision may be used) */
static seq_t *cache = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;

/* Next integer to test after the last stored prime (start after 2) */
static idx_t last_checked = 2;

/* Ensure the primes array can hold `needed` elements */
static void ensure_prime_cap(size_t needed)
{
    if (primes_cap < needed) {
        size_t new_cap = (primes_cap == 0) ? 8 : primes_cap << 1;
        while (new_cap < needed) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        primes = tmp;
        primes_cap = new_cap;
    }
}

/* Ensure the cache array can hold `needed` elements */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Test if n is prime using the primes known so far */
static bool is_prime_uint64(idx_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        /* stop when p*p > n ; avoid overflow */
        if ((unsigned __int128)p * (unsigned __int128)p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* One‑time initialization of the prime list (prime 2 is stored) */
static void init(void)
{
    if (primes == NULL) {
        primes_cap = 8;
        primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
        primes_cnt = 0;
        primes[0] = 2;
        primes_cnt = 1;

        cache_cap = 0;
        cache = NULL;
    }
}

/* Return the n-th term of the sequence (n>=1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, harmless */
    if (primes == NULL) init();

    /* generate qualifying primes until we have at least n of them */
    while (cache_cnt < (size_t)n) {
        ++last_checked;                                  /* next candidate */
        if (is_prime_uint64(last_checked)) {
            /* store prime for future divisibility checks */
            ensure_prime_cap(primes_cnt + 1);
            primes[primes_cnt++] = last_checked;

            if (good_jacobi(last_checked)) {
                ensure_cache_cap(cache_cnt + 1);
                cache[cache_cnt++] = (seq_t)last_checked;
            }
        }
    }
    return cache[(size_t)n - 1];
}
