/*
 * A049554  Primes p such that x^22 = 2 has a solution mod p.
 * https://oeis.org/A049554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_u64, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assymptotically, a(n) is the n‑th prime p (with p=2 counted) such that the congruence
   x^22 ≡ 2 (mod p) admits a solution.
   For primes p>2 this holds iff 2^((p-1)/gcd(22,p-1)) ≡ 1 (mod p).  */

/* Use a simple prime test from the utils library.  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/gcd_u64.h"

/* Euclidean gcd for uint64_t, used in the test.  */

/* Cache of the primes that satisfy the condition.  */
static seq_t *cache      = NULL;      /* pointer to the cached values   */
static size_t cache_cap  = 0;          /* capacity of the cache         */
static size_t cache_len  = 0;          /* number of stored terms        */
static idx_t   last_try  = 1;          /* last integer examined         */

/* Return the n‑th term of the sequence.  */
seq_t a(idx_t n) {
    /* Expand the cache until it contains the n‑th term.  */
    while (cache_len < n) {
        /* Find the next integer to test.  */
        last_try++;

        /* Skip composites.  */
        if (!is_prime(last_try))
            continue;

        /* Handle the special prime p = 2.  */
        if (last_try == 2) {
            /* Allocate storage if necessary.  */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)2;
            continue;
        }

        /* For odd primes p > 2.  */
        uint64_t p   = last_try;
        uint64_t g   = gcd_u64(22ull, p - 1ull);   /* gcd(22, p-1) */
        uint64_t exp = (p - 1ull) / g;             /* exponent in the test */

        seq_t rem = mod_pow((seq_t)2, exp, (seq_t)p);
        if (rem == (seq_t)1) {                    /* the root exists */
            /* Allocate storage if necessary.  */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }
    }
    return cache[n - 1];
}
