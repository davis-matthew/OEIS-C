/*
 * A086861  Floor( p/log(p) ), where p = prime(n).
 * https://oeis.org/A086861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( p / log(p) ), where p = prime(n)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   _prime_limit_for_idx, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/sieve_primes.h"

static idx_t *g_primes = NULL;      /* cached prime list */
static size_t g_primes_cnt = 0;     /* number of cached primes */
static idx_t  g_limit = 0;          /* upper bound used by the sieve */

/* Ensure that at least n primes are available in g_primes */
static void
ensure_primes(idx_t n)
{
    if (n == 0)
        return;

    if (g_primes_cnt >= n)
        return;          /* already have enough */

    /* Determine an upper bound that surely contains the nth prime. */
    size_t limit = _prime_limit_for_idx(n);

    /* Rebuild the sieve if the previous limit is too small. */
    if (g_limit < limit) {
        if (g_primes != NULL) {
            free(g_primes);
            g_primes = NULL;
        }
        size_t cnt = 0;
        sieve_primes(limit, &g_primes, &cnt);
        g_primes_cnt = cnt;
        g_limit = limit;
    } else if (g_primes_cnt < n) {
        /* Very unlikely, but handle the case where the previous limit
           was enough but fewer primes were found (sanity guard). */
        size_t cnt = 0;
        sieve_primes(limit, &g_primes, &cnt);
        g_primes_cnt = cnt;
        g_limit = limit;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n = 0; defensive */

    ensure_primes(n);
    /* prime(n) is stored in the (n-1)-th position of g_primes */
    uint64_t p = g_primes[n - 1];

    /* Compute floor(p / ln(p)) using double precision. */
    double val = p / log((double)p);

    /* Convert to a 64‑bit integer and then to seq_t.  The result fits
       comfortably in 64 bits because p ≤ 2^64 − 1. */
    uint64_t res = (uint64_t)(val + 1e-12);   /* add tiny epsilon to avoid
                                                rounding down on exact values */

    return (seq_t)res;
}
