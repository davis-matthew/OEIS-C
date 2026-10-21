/*
 * A065358  The Jacob's Ladder sequence: a(n) = Sum_{k=1..n} (-1)^pi(k), where
 *          pi = A000720.
 * https://oeis.org/A065358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, nice, changed
 * Formula:   a(n) = Sum_{k=1..n} (-1)^pi(k), a(0)=0 where pi(k) is the prime
 *            counting function
 * Range:     i8 n <= 5934, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* Cached data for memoization */
static idx_t cached_limit = 0;          /* current size (n+1) that arrays cover */
static idx_t *cached_pi = NULL;         /* pi array: number of primes <= i */
static seq_t *cached_a = NULL;          /* a array: requested sequence values */
static unsigned char *cached_isprime = NULL; /* temporary array for sieve */

/* Recomputes all data up to position n (inclusive) */
static void recompute_upto(idx_t n)
{
    /* Allocate or reallocate arrays */
    cached_pi = (idx_t *)realloc(cached_pi, (n + 1) * sizeof(idx_t));
    cached_a = (seq_t *)realloc(cached_a, (n + 1) * sizeof(seq_t));
    cached_isprime = (unsigned char *)realloc(cached_isprime,
                                              (n + 1) * sizeof(unsigned char));
    if (!cached_pi || !cached_a || !cached_isprime)
        exit(1); /* out‑of‑memory */

    /* Simple Sieve of Eratosthenes up to n */
    for (idx_t i = 0; i <= n; ++i)
        cached_isprime[i] = 1;
    cached_isprime[0] = cached_isprime[1] = 0;
    idx_t limit = (idx_t)(sqrt((double)n));
    for (idx_t p = 2; p <= limit; ++p) {
        if (cached_isprime[p]) {
            for (idx_t j = p * p; j <= n; j += p)
                cached_isprime[j] = 0;
        }
    }

    /* Compute pi(i) for all i */
    cached_pi[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        cached_pi[i] = cached_pi[i - 1] + (cached_isprime[i] ? 1 : 0);

    /* Compute a(i) from a(i-1) + (-1)^pi(i) */
    cached_a[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sign = (seq_t)((cached_pi[i] & 1) ? -1 : 1);
        cached_a[i] = cached_a[i - 1] + sign;
    }

    cached_limit = n + 1;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n + 1 > cached_limit)
        recompute_upto(n);
    return cached_a[n];
}
