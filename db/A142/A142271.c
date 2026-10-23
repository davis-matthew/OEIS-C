/*
 * A142271  Primes congruent to 22 mod 43.
 * https://oeis.org/A142271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 153, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A142271: Primes congruent to 22 (mod 43)
 * The sequence is defined as the increasing list of primes p such that
 * p ≡ 22 (mod 43).  The nth element a(n) can be found by generating
 * successive primes that satisfy this congruence.  The implementation
 * generates terms on demand, caching them so repeated calls are fast.
 * It supports both fixed‑size unsigned integer types and arbitrary‑precision
 * seq_t (via the helper is_prime_big).  The search uses only integer
 * arithmetic and the primality test; no floating point is used.
 *
 * The first few terms (for reference) are:
 * 151, 409, 839, 1097, 1613, 1699, 1871, 2129, 2473, …
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

/* static cache for computed terms */
static seq_t *cache = NULL;   /* array of computed primes */
static idx_t cache_cap = 0;   /* allocated capacity */
static idx_t cache_len = 0;   /* number of elements stored */

/*
 * Expand the cache to at least 'need' elements, generating new terms
 * as required.  The function calls is_prime_big to test primality.
 */
static void ensure_cache(idx_t need)
{
    if (cache == NULL) {
        /* initial allocation */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* allocation failure – abort program */
            abort();
        }
        /* first term is 151 */
        cache[0] = (seq_t)151;
        cache_len = 1;
    }

    while (cache_len < need) {
        /* generate next candidate congruent to 22 (mod 43) */
        seq_t cand = cache[cache_len - 1] + (seq_t)43;
        /* find the next prime in this residue class */
        while (!is_prime_big(cand))
            cand += (seq_t)43;
        /* enlarge cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;  /* double size */
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (cache == NULL) abort();
        }
        cache[cache_len++] = cand;
    }
}

/* MAIN FUNCTION TO EXPORT */
seq_t a(idx_t n)
{
    if (n < 1) 
        return (seq_t)0;   /* undefined for n = 0, but harmless */
    ensure_cache(n);
    return cache[n - 1];
}
