/*
 * A054806  Third term of strong prime quartets: prime(m+1)-prime(m) >
 *          prime(m+2)-prime(m+1) > prime(m+3)-prime(m+2).
 * https://oeis.org/A054806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 900, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the third prime of the nth strong prime quartet
   (prime(m+1)-prime(m) > prime(m+2)-prime(m+1) > prime(m+3)-prime(m+2))
   The function generates primes incrementally with next_prime and
   stores only the discovered quartet third primes. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_prime.h"

/* Cache of discovered third primes. */
static seq_t *threes = NULL;
static size_t threes_sz = 0;      /* number of stored third primes */
static size_t threes_cap = 0;     /* allocated capacity */

/* To keep memory low we store only the last four primes. */
static uint64_t last4[4] = {2, 3, 5, 7};   /* primes p1,p2,p3,p4 (oldest to newest) */
static uint64_t last_gen = 7;              /* highest prime generated */

/* Ensure the cache can hold at least `need` elements. */
static void
ensure_cache_capacity(size_t need)
{
    if (need <= threes_cap)
        return;
    size_t new_cap = threes_cap ? threes_cap * 2 : 8;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(threes, new_cap * sizeof(seq_t));
    threes = tmp;
    threes_cap = new_cap;
}

/* a(n) : return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1; n=0 is undefined */

    /* Grow the cache until we have at least n recorded terms. */
    while (threes_sz < (size_t)n) {
        /* Generate the next prime after the current highest. */
        uint64_t p = next_prime(last_gen);
        last_gen = p;

        /* Shift the window of the last four primes. */
        last4[0] = last4[1];
        last4[1] = last4[2];
        last4[2] = last4[3];
        last4[3] = p;

        /* Check the decreasing‑gap condition. */
        uint64_t g1 = last4[1] - last4[0];  /* p2 - p1 */
        uint64_t g2 = last4[2] - last4[1];  /* p3 - p2 */
        uint64_t g3 = last4[3] - last4[2];  /* p4 - p3 */
        if (g1 > g2 && g2 > g3) {
            /* Record the third prime (p3). */
            seq_t third = (seq_t)last4[2];
            ensure_cache_capacity(threes_sz + 1);
            threes[threes_sz++] = third;
        }
    }
    return threes[n - 1];
}
