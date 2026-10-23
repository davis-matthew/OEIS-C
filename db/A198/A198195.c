/*
 * A198195  a(n) is the smallest prime(m) such that the interval (prime(m)*n,
 *          prime(m+1)*n) contains exactly five primes.
 * https://oeis.org/A198195
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 93, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A198195: a(n) = the smallest prime pₘ such that the interval
   (pₘ·n , pₘ₊₁·n) contains exactly five primes.
   Computation uses a growing sieve of primes and a linear scan
   over consecutive prime pairs. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_primes.h"   /* sieve_primes(limit, &primes, &cnt) */

/* --- helper binary searches on a sorted array of idx_t primitives --- */
static size_t first_gt(const idx_t *arr, size_t n, idx_t val) {
    size_t l = 0, r = n;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if (arr[m] <= val)
            l = m + 1;
        else
            r = m;
    }
    return l;           /* first index with arr[i] > val, may equal n */
}

static size_t first_ge(const idx_t *arr, size_t n, idx_t val) {
    size_t l = 0, r = n;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if (arr[m] < val)
            l = m + 1;
        else
            r = m;
    }
    return l;           /* first index with arr[i] >= val, may equal n */
}

/* --- main sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2u)           /* not defined below offset; return 0 as fallback */
        return (seq_t)0;

    /* static prime table shared between calls */
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t sieve_limit = 0;

    /* initial sieve on first invocation */
    if (primes == NULL) {
        sieve_limit = 1000000; /* start with 1e6; increase as needed */
        sieve_primes(sieve_limit, &primes, &prime_cnt);
    }

    for (;;) { /* repeat until the required prime is found */
        for (size_t i = 0; i + 1 < prime_cnt; ++i) {
            /* compute interval limits: (p_i * n , p_{i+1} * n) */
            unsigned __int128 lo128 = (unsigned __int128)primes[i] * n;
            unsigned __int128 hi128 = (unsigned __int128)primes[i + 1] * n;
            idx_t low  = (idx_t)lo128;
            idx_t high = (idx_t)hi128;

            /* if high exceeds the largest sieve prime, break to enlarge sieve */
            if (high > primes[prime_cnt - 1]) {
                /* enlarge sieve limit geometrically */
                while (high > primes[prime_cnt - 1]) {
                    sieve_limit <<= 1; /* double */
                    /* re-sieve, freeing the old array */
                    idx_t *tmp = NULL;
                    size_t tmpcnt = 0;
                    sieve_primes(sieve_limit, &tmp, &tmpcnt);
                    free(primes);
                    primes = tmp;
                    prime_cnt = tmpcnt;
                }
                /* restart scanning from the beginning after resizing */
                break;
            }

            /* count primes strictly between low and high */
            size_t l = first_gt(primes, prime_cnt, low);   /* first > low */
            size_t r = first_ge(primes, prime_cnt, high);  /* first >= high */
            if (r == 0)
                continue;           /* high < smallest prime, impossible here */

            r--; /* now r is last index with prime < high */

            if (l <= r) {
                seq_t count = (seq_t)(r - l + 1);
                if (count == 5u)
                    return (seq_t)primes[i];
            }
        }

        /* No suitable prime found with current sieve limit: enlarge and retry */
        sieve_limit <<= 1; /* double */
        idx_t *tmp = NULL;
        size_t tmpcnt = 0;
        sieve_primes(sieve_limit, &tmp, &tmpcnt);
        free(primes);
        primes = tmp;
        prime_cnt = tmpcnt;
        /* loop again with new expanded table */
    }
}
