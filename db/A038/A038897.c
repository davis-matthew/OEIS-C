/*
 * A038897  23 is a square mod p.
 * https://oeis.org/A038897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 23 is a quadratic residue modulo p.
   Computed by testing successive primes for the Legendre symbol using
   quadratic reciprocity.  No closed‑form formula is known. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static const bool residue[23] = {
    false, /* 0 */
    true,  /* 1 */
    true,  /* 2 */
    true,  /* 3 */
    true,  /* 4 */
    false, /* 5 */
    true,  /* 6 */
    false, /* 7 */
    true,  /* 8 */
    true,  /* 9 */
    false, /*10*/
    false, /*11*/
    true,  /*12*/
    true,  /*13*/
    false, /*14*/
    false, /*15*/
    true,  /*16*/
    false, /*17*/
    true,  /*18*/
    false, /*19*/
    false, /*20*/
    false, /*21*/
    false  /*22*/
};

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_len = 0;      /* number of values stored */
static size_t cache_cap = 0;      /* capacity of storage */
static idx_t next_candidate = 2;  /* next integer to test */

/* Append a value to the cache, reallocating if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap ? cache_cap * 2 : 16);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Test whether a prime p satisfies that 23 is a quadratic residue mod p. */
static bool qualifies_p(idx_t p)
{
    if (p == 2) return true;          /* 23 mod 2 = 1 */
    if (p == 23) return true;         /* 23 ≡ 0 mod 23 */

    idx_t pm23 = p % 23;
    bool is_resid = residue[pm23];
    if (p % 4 == 1) {
        return is_resid;
    }
    /* p % 4 == 3 */
    return !is_resid;
}

/* Return the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* guard, not actually used */

    /* Generate terms until we have at least n. */
    while (cache_len < n) {
        /* Find next prime. */
        while (!is_prime(next_candidate)) {
            next_candidate++;
        }
        /* Check the quadratic residue condition. */
        if (qualifies_p(next_candidate)) {
            cache_append((seq_t)next_candidate);
        }
        next_candidate++;
    }
    return cache[n - 1];
}
