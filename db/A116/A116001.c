/*
 * A116001  sigma(n) - phi(n) gives a semiprime (A001358).
 * https://oeis.org/A116001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 126, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, isPrimeGeneric, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that sigma(n) - phi(n) is a semiprime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/phi.h"
#include "../utils/sigma_of.h"
#include "../utils/floor_sqrt.h"
#include "../utils/isPrimeGeneric.h"

/* ---------- Helper: check whether a value is semiprime ---------- */
static bool is_semiprime(seq_t x)
{
    if (x <= 1) return false;            /* 0 or 1 is not a semiprime */

    /* Primes are not semiprime */
    if (isPrimeGeneric(x)) return false;

    /* Compute integer sqrt of x */
    seq_t sqrt_seq = floor_sqrt(x);
    uint64_t sqrt_val = (uint64_t)sqrt_seq;   /* fits in 64 bits for test n ≤ 10000 */

    /* Trial division up to sqrt(x) */
    for (uint64_t p = 2; p <= sqrt_val; ++p) {
        seq_t sp = (seq_t)p;
        if (x % sp == 0) {
            seq_t q = x / sp;
            /* x is semiprime iff quotient is prime */
            return isPrimeGeneric(q);
        }
    }
    return false;
}

/* ---------- Cache of already found terms ---------- */
static seq_t *term_cache = NULL;   /* array of found n */
static size_t term_len   = 0;      /* number of elements stored */
static size_t term_cap   = 0;      /* allocated capacity */

/* Append a new term to the cache, reallocating if necessary */
static void cache_append(seq_t val)
{
    if (term_len == term_cap) {
        size_t new_cap = term_cap ? term_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* malloc failure */
        term_cache = tmp;
        term_cap   = new_cap;
    }
    term_cache[term_len++] = val;
}

/* Main sequence function.  It generates terms lazily and memoises them. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* guard – sequence is 1‑indexed */

    /* Ensure we have at least n terms in the cache. */
    while (term_len < (size_t)n) {
        /* Start searching from the next integer after the last found term. */
        idx_t m = term_len ? (idx_t)(uint64_t)term_cache[term_len - 1] + 1 : 1;

        /* Find the next n such that sigma(n)-phi(n) is semiprime. */
        while (1) {
            seq_t sigma_val = sigma_of(m);
            seq_t phi_val   = phi(m);
            seq_t diff      = sigma_val - phi_val;

            if (is_semiprime(diff)) {
                cache_append((seq_t)m);
                break;
            }
            ++m;
        }
    }

    return term_cache[n - 1];
}
