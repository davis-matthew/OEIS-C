/*
 * A106064  Primes p such that 1*p + 16 and 16*p + 1 are primes.
 * https://oeis.org/A106064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that p + 16 and 16 p + 1 are also
 *            primes
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate A106064.  Cached primes allow arbitrary calling order. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"  /* bool is_prime(uint64_t n) */

static seq_t *qual = NULL;     /* cache of qualifying primes             */
static idx_t  qual_cap = 0;    /* current allocation capacity (in entries) */
static idx_t  qual_len = 0;    /* number of primes stored so far          */
static uint64_t next_candidate = 2;  /* next number to test (prime search)     */

/* Extend the cache when needed.  (Realloc requires a cast in C++). */
static void maybe_grow(void)
{
    if (qual_len >= qual_cap) {
        idx_t new_cap = qual_cap ? qual_cap << 1 : 16;
        qual = (seq_t *)realloc((void *)qual, new_cap * sizeof(seq_t));
        qual_cap = new_cap;
    }
}

/* Return the nth term (n ≥ 1).  The function supports out‑of‑order calls. */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n in the cache. */
    while (qual_len < n) {
        maybe_grow();

        /* Test primality of the candidate. */
        if (is_prime(next_candidate)) {
            /* Check the two required conditions. */
            if (is_prime(next_candidate + 16ULL) &&
                is_prime(16ULL * next_candidate + 1ULL)) {
                /* Store the qualifying prime. */
                qual[qual_len++] = (seq_t)next_candidate;
            }
        }

        /* Advance to the next candidate (skip even numbers after 2). */
        if (next_candidate == 2ULL)
            next_candidate = 3ULL;
        else
            next_candidate += 2ULL;
    }

    /* Return the required term. */
    return qual[n - 1];
}
