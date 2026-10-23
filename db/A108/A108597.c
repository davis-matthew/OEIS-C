/*
 * A108597  Numbers n such that 911*n - 11 is prime.
 * https://oeis.org/A108597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of integers n such that 911*n – 11 is prime. */
/* The function memoises already found terms and produces the n‑th term
   on demand.  It works for any seq_t (unsigned 8–128 bit or an
   arbitrary‑precision integer class) because all terms are stored
   internally as uint64_t and cast to seq_t only when returned. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime64.h"

static uint64_t *terms64 = NULL;   /* array of terms in uint64_t form */
static size_t   terms_cap = 0;     /* current capacity of terms64          */
static idx_t    terms_count = 0;   /* number of terms already found        */
static uint64_t next_candidate = 2;/* next integer to test as a term       */

/* Ensure the internal array has capacity at least min_cap. */
static void ensure_terms(size_t min_cap)
{
    if (terms_cap < min_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 64;
        if (new_cap < min_cap) new_cap = min_cap;
        uint64_t *tmp = (uint64_t *)realloc(terms64, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);            /* fatal on allocation failure */
        terms64 = tmp;
        terms_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;          /* undefined for n <= 0 */

    while (terms_count < n) {
        /* compute 911 * candidate - 11 without overflow for normal usage */
        unsigned __int128 tmp = (unsigned __int128)911 * next_candidate;
        uint64_t val = (uint64_t)(tmp - 11);

        if (is_prime64(val)) {
            ensure_terms(terms_count + 1);
            terms64[terms_count] = next_candidate;
            terms_count++;
        }
        next_candidate++;
    }
    return (seq_t)terms64[n - 1];
}
