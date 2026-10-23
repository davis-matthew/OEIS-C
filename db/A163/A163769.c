/*
 * A163769  Primes p such that 2*p+3 is not prime.
 * https://oeis.org/A163769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate successive primes p and keep those for which
   2*p+3 is composite.  The results are memoised so that calls to a(n)
   can arrive in any order. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"      /* uint64_t next_prime(uint64_t)   */
#include "../utils/isPrimeGeneric.h" /* bool isPrimeGeneric(seq_t)      */

/* static cache for memoised terms */
static seq_t *cache      = NULL; /* array holding computed terms          */
static size_t cache_len  = 0;     /* number of terms stored                */
static size_t cache_cap  = 0;     /* allocated capacity                    */

/* next prime that has not yet been examined */
static uint64_t next_candidate = 2;

/* ------------------------------------------------------------------ */
/*  a(idx_t n) – return the n‑th term of the sequence                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* generate terms until we have at least n entries */
    while (cache_len < n)
    {
        /* fetch the next prime */
        uint64_t p = next_prime(next_candidate);
        next_candidate = p + 1u;  /* advance for the next search */

        /* test whether 2*p+3 is composite */
        seq_t val = (seq_t)p;
        seq_t two_p_plus_three = val * 2 + 3;  /* seq_t arithmetic */

        if (!isPrimeGeneric(two_p_plus_three))
        {
            /* grow cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *newbuf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!newbuf) abort(); /* memory exhausted */
                cache = newbuf;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }

    /* return the requested term (offset 1) */
    return cache[n - 1];
}
