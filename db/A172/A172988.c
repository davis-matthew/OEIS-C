/*
 * A172988  Primes p such that either p-3 or p-6 is prime.
 * https://oeis.org/A172988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/*  A172988 – Primes p such that either p-3 or p-6 is prime.
 *  Computation: sieve primes up to a growing limit until n terms
 *  are found.  The results are cached, so repeated calls in any
 *  order run in O(1) after the first expansion.  For seq_t larger
 *  than 64 bits (arbitrary‑precision builds) this also works
 *  because we only cast the prime value to seq_t.
 *
 *  The only special case is p = 5, because for any odd prime
 *  p > 5, p-3 is even and therefore not prime.
 */

static seq_t  *term_cache  = NULL;  /* array of found terms          */
static size_t   term_len    = 0;     /* how many terms we have        */
static idx_t    sieve_limit = 0;    /* size of the last used sieve   */

/* Helper: build a boolean sieve of size 'limit' (inclusive). */
/*  The array is zero‑initialised (0 = composite, 1 = prime).    */
static uint8_t *
build_sieve(idx_t limit)
{
    uint8_t *s = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!s)
        return NULL;

    for (idx_t i = 2; i <= limit; ++i)
        s[i] = 1;                 /* assume prime */

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!s[p])
            continue;
        for (idx_t j = p * p; j <= limit; j += p)
            s[j] = 0;
    }
    return s;
}

/*  Return the nth term (1‑based).  If n == 0, return 0 (undefined). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Return cached value if we already know it. */
    if (term_cache && term_len >= n)
        return term_cache[n - 1];

    /* Grow the sieve until we have at least n terms. */
    while (term_len < n) {
        idx_t new_limit;
        if (sieve_limit == 0)
            new_limit = 200 * n; /* initial guess */
        else
            new_limit = sieve_limit * 2;

        uint8_t *s = build_sieve(new_limit);
        if (!s)
            return (seq_t)0;   /* out of memory – unexpected in tests */

        /* Count how many qualifying primes up to new_limit. */
        size_t count = 0;
        for (idx_t p = 2; p <= new_limit; ++p) {
            if (!s[p])
                continue;
            if (p == 5 || (p > 6 && s[p - 6]))
                ++count;
        }

        /* Allocate space for the terms. */
        seq_t *new_terms = (seq_t *)malloc(sizeof(seq_t) * count);
        if (!new_terms) {
            free(s);
            return (seq_t)0;   /* out of memory – unexpected */
        }

        /* Fill the array. */
        size_t idx = 0;
        for (idx_t p = 2; p <= new_limit && idx < count; ++p) {
            if (!s[p])
                continue;
            if (p == 5 || (p > 6 && s[p - 6])) {
                new_terms[idx++] = (seq_t)p;
            }
        }

        /* Replace the old cache. */
        free(s);
        free(term_cache);
        term_cache = new_terms;
        term_len   = count;
        sieve_limit = new_limit;
    }

    /* Now we have at least n terms. */
    return term_cache[n - 1];
}
