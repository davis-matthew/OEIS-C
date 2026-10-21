/*
 * A093326  Duplicate of A001359.
 * https://oeis.org/A093326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A093326: primes p such that (p + nextprime(p)) / 2 + 1 is prime
   (2 is omitted to match the OEIS listed terms). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start) */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n) */

/* static cache of qualifying primes */
static seq_t  *qual = NULL;    /* array holding the primes that satisfy the condition */
static size_t   qual_cap = 0;   /* allocated capacity */
static size_t   qual_len = 0;   /* number of primes stored */
static uint64_t curr_prime = 1; /* last prime examined (start at 1 so first next_prime is 2) */

/* make sure 'qual' has at least 'needed' slots */
static void
ensure_cap(size_t needed)
{
    if (qual_cap < needed) {
        size_t new_cap = (qual_cap == 0) ? 16 : qual_cap << 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(qual, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        qual = tmp;
        qual_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* safeguard for unexpected call */

    /* extend the cache until it contains at least n qualifying primes */
    while (qual_len < (size_t)n) {
        /* advance to the next prime */
        curr_prime = next_prime(curr_prime);
        /* skip the prime 2 to match the OEIS sequence listing */
        if (curr_prime == 2) continue;

        uint64_t nextp = next_prime(curr_prime);

        /* compute (p + nextp) / 2 + 1, using 64‑bit arithmetic */
        uint64_t mid_plus = (curr_prime + nextp) / 2 + 1;

        /* test primality of the result */
        if (is_prime_big((seq_t)mid_plus)) {
            ensure_cap(qual_len + 1);
            qual[qual_len++] = (seq_t)curr_prime;
        }
    }

    /* return the (n‑1)-th qualifying prime */
    return qual[(size_t)n - 1];
}
