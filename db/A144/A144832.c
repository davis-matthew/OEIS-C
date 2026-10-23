/*
 * A144832  Distance from nxtprm(n^2) to (n+1)^2 in A144831 is prime.
 * https://oeis.org/A144832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n‑th value of the distance d between nxtprm(n²) and
 *            (n+1)² that is a prime. The sequence starts with a(1)=2, a(2)=5,
 *            a(3)=7, …
 * Range:     u8 n <= 35, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses trial‑division primality testing via the
   existing helper is_prime(idx_t) from utils/is_prime.h.
   No floating point or sqrt calls are required, so it
   works for all build types (uint8_t … big integer). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *term_cache = NULL;        /* cached sequence terms   */
static size_t term_capacity = 0;        /* current capacity of cache */
static size_t term_count = 0;           /* number of terms stored   */
static idx_t next_gen_n = 1;            /* next n to generate       */

/* Ensure term_cache holds at least n terms. */
static void ensure_terms(idx_t n)
{
    while (term_count < n) {
        /* Compute distance for current n. */
        uint64_t nv = (uint64_t)next_gen_n;          /* n as 64‑bit integer */
        uint64_t nsq = nv * nv;                    /* n²                            */
        uint64_t p = nsq + 1;                      /* start searching for next prime */
        while (!is_prime(p)) ++p;                  /* find first prime > n²          */
        uint64_t nextsq = (nv + 1) * (nv + 1);     /* (n+1)²                        */
        uint64_t dist = nextsq - p;                /* distance                       */

        if (is_prime(dist)) {
            /* Append distance to cache. */
            if (term_count == term_capacity) {
                term_capacity = term_capacity == 0 ? 64 : term_capacity << 1;
                term_cache = (seq_t *)realloc(term_cache, term_capacity * sizeof(seq_t));
            }
            term_cache[term_count++] = (seq_t)dist;
        }
        ++next_gen_n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; a(0) is undefined */
    if (!term_cache) {
        term_capacity = 64;
        term_cache = (seq_t *)malloc(term_capacity * sizeof(seq_t));
    }
    ensure_terms(n);
    return term_cache[n - 1];
}
