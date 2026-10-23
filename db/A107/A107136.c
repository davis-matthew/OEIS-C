/*
 * A107136  Primes of the form 3x^2 + 10y^2.
 * https://oeis.org/A107136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   estimate_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107136: primes of the form 3*x^2 + 10*y^2.
   The primes are exactly the primes that are congruent to
   3, 13, 37, 43 or 67 modulo 120, so we sieve primes, keep
   those in the required residue classes and return them in order. */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/estimate_limit.h"

static uint64_t *special      = NULL;  /* primes of the form 3x^2+10y^2 */
static size_t   special_cap   = 0;
static size_t   special_count = 0;       /* how many have been found */

static unsigned char *sieve      = NULL; /* prime flag array for current limit */
static size_t        sieve_limit = 0;     /* highest number sieved */

/* ------------------------------------------------------------------ */
/* Estimate an upper bound for the n-th special prime, using the
   prime number theorem (with a safety factor).                       */

/* ------------------------------------------------------------------ */
/* Extend the current sieve up to new_limit and collect special primes
   in that range.                                                   */
/* ------------------------------------------------------------------ */
static void extend_to(size_t new_limit)
{
    if (new_limit <= sieve_limit) return;   /* nothing to do            */

    /* Allocate a fresh sieve array for the new range. */
    unsigned char *is_comp =
        (unsigned char *)malloc(new_limit + 1);
    if (!is_comp) return;                    /* malloc failure → give up */
    memset(is_comp, 0, new_limit + 1);

    /* Standard Eratosthenes sieve. */
    size_t limit_sqrt = (size_t)sqrt((double)new_limit);
    for (size_t p = 2; p <= limit_sqrt; ++p) {
        if (!is_comp[p]) {
            for (size_t q = p * p; q <= new_limit; q += p)
                is_comp[q] = 1;
        }
    }

    /* Collect special primes in the new range. */
    for (size_t n = sieve_limit + 1; n <= new_limit; ++n) {
        if (!is_comp[n]) {                      /* n is prime           */
            uint8_t r = n % 120;
            if (r == 3 || r == 13 || r == 37 ||
                r == 43 || r == 67) {
                /* Ensure capacity of the result array. */
                if (special_count == special_cap) {
                    size_t new_cap = special_cap == 0 ? 128 : special_cap * 2;
                    uint64_t *tmp = (uint64_t*)realloc(special,
                                                        new_cap * sizeof(uint64_t));
                    if (!tmp) {                     /* realloc failure → give up */
                        free(is_comp);
                        return;
                    }
                    special = tmp;
                    special_cap = new_cap;
                }
                special[special_count++] = n;
            }
        }
    }

    free(is_comp);
    sieve_limit = new_limit;
}

/* ------------------------------------------------------------------ */
/* Public entry point.  Returns the n-th prime of the form
   3*x^2 + 10*y^2.  The sequence is 1‑based (a(1) = 3).           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range; offset is 1 */

    /* Generate more special primes until we have at least n. */
    while (special_count < (size_t)n) {
        size_t new_limit = estimate_limit((idx_t)(special_count + 10));
        if (new_limit <= sieve_limit)
            new_limit = sieve_limit * 2 + 1000;   /* force an expansion     */
        extend_to(new_limit);

        /* If after expansion we still don't have enough, expand again. */
        if (special_count >= (size_t)n) break;
    }

    if (special_count < (size_t)n) return 0;   /* should not happen */

    return (seq_t)special[n - 1];   /* zero‑based array into 1‑based index */
}
