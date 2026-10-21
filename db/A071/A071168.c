/*
 * A071168  n-th prime reduced modulo phi(n).
 * https://oeis.org/A071168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = prime(n) mod phi(n)
 * Range:     u8 n <= 54, u16 n <= 7532, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/phi_uint64.h"

/* Global storage for the list of primes generated so far. */
static idx_t *prime_list   = NULL;   /* 0‑based array of primes */
static size_t prime_count  = 0;      /* how many primes are in prime_list */
static idx_t prime_limit   = 0;      /* maximum value that has been sieved */

/* Ensure that prime_list contains at least 'needed' primes.
   The sieve bounds are increased in multiples of 20; this bound is
   safe for n up to at least 10 000. */
static void
ensure_prime_count(idx_t needed)
{
    if (prime_count >= needed)
        return;                 /* we already have enough primes */

    /* Grow the reserve for the prime array. */
    prime_list = (idx_t *)realloc(prime_list,
                                  (size_t)needed * sizeof(idx_t));
    if (!prime_list)
        exit(1);
    prime_count = 0;

    /* Choose a sieve limit.  If a previous sieve already covered a
       larger range we reuse it, otherwise we grow the bound. */
    idx_t limit = needed * 20;
    if (limit < 100)                 /* guard for very small n */
        limit = 100;
    if (prime_limit > limit)
        limit = prime_limit;         /* keep the older, larger bound */

    unsigned char *is_comp = (unsigned char *)
        calloc((size_t)(limit + 1), 1);
    if (!is_comp)
        exit(1);

    for (idx_t i = 2; i <= limit; ++i)
    {
        if (!is_comp[i])
        {
            if (prime_count < needed)
                prime_list[prime_count++] = i;
            if (i * i <= limit)
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
        }
    }
    free(is_comp);
    prime_limit = limit;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;         /* undefined, but returns 0 safely */

    /* Make sure we have at least n primes available. */
    ensure_prime_count(n);

    /* nth prime (enumerated from 1). */
    seq_t prime_n = (seq_t)prime_list[n - 1];

    /* Euler's totient φ(n).  The helper returns uint64_t,
       which fits into seq_t for the values of n used here. */
    seq_t phi_n = (seq_t)phi_uint64(n);

    /* Compute the required residue. */
    seq_t result = prime_n % phi_n;
    return result;
}
