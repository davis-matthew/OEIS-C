/*
 * A090262  Numbers n such that the strings n1n and 1n1 are both prime.
 * https://oeis.org/A090262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 738, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits, isPrimeGeneric, pow10, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): numbers n such that n1n and 1n1 are both prime. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/digits.h"
#include "../utils/seq_realloc.h"

/* Cache of found terms */
static seq_t *cache          = NULL;   /* array of found values */
static size_t cache_len      = 0;      /* number of values stored */
static size_t cache_cap      = 0;      /* capacity of the array */
static idx_t  next_candidate = 1;      /* candidate n to test next */

/* Append val to the cache, growing it if necessary */
static void push_cache(idx_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t)val;
}

/* a(n) returns the nth term (offset 1) */
seq_t a(idx_t n)
{
    /* Extend cache until we have at least n terms */
    while (cache_len < n) {
        /* Find next candidate that satisfies the prime condition */
        while (true) {
            idx_t cand = next_candidate++;
            int d = digits(cand);                     /* number of decimal digits of cand */

            /* Pre‑compute powers of 10 */
            seq_t pow10_d1 = pow10((idx_t)(d + 1));

            /* n1n = n * 10^(d+1) + 10^d + n   */
            seq_t n1n = ((seq_t)cand) * pow10_d1 + pow10((idx_t)d) + (seq_t)cand;

            /* 1n1 = 10^(d+1) + n * 10 + 1   */
            seq_t one_n1 = pow10_d1 + ((seq_t)cand) * (seq_t)10 + (seq_t)1;

            if (isPrimeGeneric(n1n) && isPrimeGeneric(one_n1)) {
                push_cache(cand);
                break;          /* move to next term */
            }
        }
    }
    return cache[n - 1];
}
