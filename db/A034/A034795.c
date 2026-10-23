/*
 * A034795  a(n) is the least prime > a(n-1) that is a quadratic residue mod
 *          a(n-1).
 * https://oeis.org/A034795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 3510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2; for n>1, a(n) is the least prime > a(n‑1) that
   is a quadratic residue modulo a(n‑1). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/mod_pow.h"     /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

#define MAX_CACHE 200000        /* enough for n up to ~10⁶ in practice */

static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;    /* number of terms stored */

/* Compute a(n) (1‑based), storing intermediate results in cache */
seq_t
a(idx_t n)
{
    if (n == 0)                      /* undefined, return 0 */
        return 0;

    if (cache_len == 0) {            /* initialise with a(1)=2 */
        cache[0] = (seq_t)2;
        cache_len = 1;
    }

    while (cache_len < n) {          /* compute up to term n */
        if (cache_len >= MAX_CACHE)   /* safety check */
            abort();

        seq_t prev = cache[cache_len - 1];           /* a(k-1) */
        /* prev is always prime; cast it to uint64_t for exp */
        uint64_t prev64 = (uint64_t)prev;
        uint64_t exp = (prev64 - 1) >> 1;            /* (prev-1)/2 */

        /* first prime strictly greater than prev */
        uint64_t candidate = next_prime((uint64_t)prev);

        while (1) {
            seq_t cand = (seq_t)candidate;
            seq_t leg = mod_pow(cand, exp, prev);   /* Legendre symbol */
            if (leg == (seq_t)1) {
                cache[cache_len] = cand;            /* found a(k) */
                cache_len++;
                break;
            }
            /* try the next prime */
            candidate = next_prime((uint64_t)candidate);
        }
    }

    return cache[n - 1];
}
