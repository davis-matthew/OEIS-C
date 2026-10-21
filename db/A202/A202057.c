/*
 * A202057  Numbers which are not perfect squares and such that all prime
 *          divisors are congruent to 1 or 2 mod 4.
 * https://oeis.org/A202057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 3407, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A202057: numbers which are not perfect squares and whose prime
   divisors are all congruent to 1 or 2 modulo 4.  The function
   searches integers incrementally, keeping a cache so that any
   permutation of calls to a(n) is supported. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* --- local helper: test if x satisfies the condition ------------------- */
static bool valid_term(seq_t x)
{
    /* reject 1 and perfect squares */
    if (x <= 1) return false;
    seq_t r = floor_sqrt(x);
    if (r * r == x) return false;

    /* remove factor 2 (allowed) */
    while ((x % 2) == 0) x /= 2;

    /* test odd prime factors */
    seq_t p = 3;
    while (p * p <= x) {
        if (x % p == 0) {
            if ((p % 4) == 3)   /* forbidden prime */
                return false;
            while (x % p == 0) x /= p;
        }
        p += 2;
    }

    /* remaining factor (if any) must not be ≡3 (mod 4) */
    if (x > 1 && (x % 4) == 3) return false;

    return true;
}

/* --- cache management ---------------------------------------------------- */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static seq_t next_candidate = 2; /* first integer to test */

/* append a new value, expanding storage as needed */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* --- public entry point ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, under offset 1 */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t cand = next_candidate;
        while (!valid_term(cand)) ++cand;
        cache_append(cand);
        next_candidate = cand + 1;
    }
    return cache[n - 1];
}
