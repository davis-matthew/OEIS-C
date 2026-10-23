/*
 * A137238  Primes which contain the digit 1 or the digit 2.
 * https://oeis.org/A137238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth prime that contains the decimal digit 1 or 2
 * Range:     u8 n <= 40, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   has_digit1or2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (indexing starts at 1, a(1) = 2) */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_digit1or2.h"

/*--- static state -------------------------------------------------------*/
static seq_t *cache      = NULL;   /* array of found sequence values    */
static size_t cache_len  = 0;      /* number of values stored          */
static size_t cache_cap  = 0;      /* allocated capacity of cache      */

static uint64_t *prime_list = NULL; /* list of primes found so far      */
static size_t    prime_len  = 0;     /* number of primes in the list     */
static size_t    prime_cap  = 0;     /* allocated capacity of prime_list */

static uint64_t next_candidate = 2;  /* next integer to test for primality */

/* push a new prime into prime_list, reallocating if necessary */
static void push_prime(uint64_t p)
{
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap? prime_cap << 1 : 128;
        uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();                 /* memory allocation failed */
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_len++] = p;
}

/* simple trial division using the growing list of primes */
static bool is_prime_small(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1) == 0) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* allocate or grow cache array to hold at least n elements */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;                /* already enough */

    /* allocate cache buffer on first call */
    if (!cache) {
        cache_cap = 256;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                           /* allocation fail */
    }
    /* grow if necessary */
    if (n > cache_cap) {
        while (n > cache_cap) cache_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* generate primes and fill cache until we have n terms */
    while (cache_len < (size_t)n) {
        /* find next prime */
        while (!is_prime_small(next_candidate)) ++next_candidate;
        /* prime found, record it */
        push_prime(next_candidate);
        if (has_digit1or2(next_candidate))
            cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* the requested sequence function ------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n==0, return 0 */
    ensure_cache(n);
    return cache[n-1];             /* a(1) stored at cache[0] */
}
