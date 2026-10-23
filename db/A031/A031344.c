/*
 * A031344  Write primes in base 10 but interpret as if in base 12.
 * https://oeis.org/A031344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret the decimal digits of the n-th prime as a
 *            base‑12 number. This is OEIS sequence A031344
 * Range:     u8 n <= 44, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "../utils/is_prime.h"     /* provides bool is_prime(idx_t x); */

/* Static cache of primes (1‑based indexing). */
static uint64_t *prime_cache = NULL;
static idx_t prime_cache_n   = 0;     /* number of cached primes */
static idx_t prime_cache_cap = 0;     /* allocated capacity */
static uint64_t prime_candidate = 2;  /* next integer to test for primality */

/* Ensure at least n primes are cached. */
static void
ensure_cache(idx_t n)
{
    if (prime_cache_n >= n)
        return;

    /* enlarge cache if needed */
    if (prime_cache_cap < n) {
        idx_t new_cap = (prime_cache_cap == 0) ? 16 : prime_cache_cap * 2;
        if (new_cap < n)
            new_cap = n;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
        if (!tmp) /* allocation failed – caller will handle */
            return;
        prime_cache = tmp;
        prime_cache_cap = new_cap;
    }

    /* generate primes until we have n of them */
    while (prime_cache_n < n) {
        if (prime_candidate == 2 || is_prime(prime_candidate))
            prime_cache[prime_cache_n++] = prime_candidate;
        ++prime_candidate;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1, behaviour for n==0 is undefined */

    ensure_cache(n);

    uint64_t p = prime_cache[n - 1];  /* the n‑th prime */

    /* convert decimal digits of p to a base‑12 value */
    char buf[32];                     /* works for all 64‑bit integers */
    int len = 0;
    uint64_t tmp = p;
    do {
        buf[len++] = '0' + (int)(tmp % 10);
        tmp /= 10;
    } while (tmp > 0);

    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * (seq_t)12 + (seq_t)(buf[i] - '0');
    }
    return result;
}
