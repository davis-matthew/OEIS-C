/*
 * A127066  a(0)=1; for n > 0, a(n) = a(n-1) + a(prime(n)(mod n)), where
 *          prime(n) is the n-th prime.
 * https://oeis.org/A127066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; for n > 0, a(n) = a(n-1) + a(prime(n) mod n) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_primes.h"

static seq_t *a_cache = NULL;          /* memoized a(n) values */
static size_t a_len   = 0;             /* number of stored elements (a_len-1 is max index) */

static idx_t *primes   = NULL;         /* array of primes, 1‑based: primes[0] = 2 */
static idx_t primes_len   = 0;         /* number of primes currently stored */
static idx_t prime_limit  = 0;         /* last sieve limit used */

/* Ensure that primes array contains at least n primes (prime(n) needed) */
static void ensure_primes(idx_t n) {
    if (primes_len >= n)
        return;

    idx_t limit = (prime_limit == 0) ? 100 : prime_limit * 2 + 1000;
    while (1) {
        size_t cnt = 0;
        idx_t *pr = NULL;
        sieve_primes(limit, &pr, &cnt);   /* generates primes <= limit */
        if (cnt >= n) {
            free(primes);                  /* free old array */
            primes     = pr;
            primes_len = cnt;
            prime_limit = limit;
            break;
        }
        free(pr);
        limit *= 2;                        /* increase the limit */
    }
}

/* Ensure that a_cache contains values up to at least index n */
static void ensure_cache(idx_t n) {
    if (a_len > 0 && (idx_t)(a_len - 1) >= n)
        return;

    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();  /* allocation failure */
    a_cache = tmp;

    /* Initialize freshly allocated entries if any */
    for (size_t i = a_len; i < new_cap; ++i)
        a_cache[i] = 0;

    if (a_len == 0) {
        a_cache[0] = (seq_t)1; /* a(0) = 1 */
        a_len = 1;
    }
}

/* Compute a(n) with memoisation and on‑demand prime servicing */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    ensure_cache(n);
    ensure_primes(n);

    for (idx_t i = a_len; i <= n; ++i) {
        if (i == 0) continue; /* already set */

        seq_t prev = a_cache[(size_t)(i - 1)];
        idx_t idx = (idx_t)(primes[(size_t)i - 1] % i);  /* prime(i) mod i */
        seq_t term = prev + a_cache[(size_t)idx];
        a_cache[(size_t)i] = term;
    }
    a_len = (size_t)n + 1;
    return a_cache[(size_t)n];
}
