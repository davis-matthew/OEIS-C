/*
 * A132255  Isolated primes congruent to {17, 19} mod 30.
 * https://oeis.org/A132255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th isolated prime that is ≡ 17 or 19 (mod 30) and
 *            whose neighbours p ± 2 are not prime
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force search with caching, using the helper
   is_prime() from the utilities. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

static seq_t *term_cache = NULL;
static idx_t term_size   = 0; /* number of terms stored */
static idx_t term_cap    = 0; /* current cache capacity */

/* grow the cache to accommodate at least new_cap terms */
static void grow_cache(idx_t new_cap)
{
    if (term_cap >= new_cap)
        return;
    idx_t cap = term_cap ? term_cap * 2 : 32;
    while (cap < new_cap)
        cap *= 2;
    term_cache = (seq_t *)realloc(term_cache, cap * sizeof(seq_t));
    term_cap   = cap;
}

seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for n < 1; return 0 for safety */
        return (seq_t)0;

    /* Return cached value if already computed */
    if (term_size >= n)
        return term_cache[n - 1];

    /* Start searching from the first term if none cached, otherwise
       from the next odd number after the last cached term. */
    uint64_t candidate;
    if (term_size == 0)
        candidate = 47ULL;        /* first element of the sequence */
    else
        candidate = (uint64_t)term_cache[term_size - 1] + 2ULL;  /* next odd */

    /* Ensure we always work with odd candidates */
    if ((candidate & 1ULL) == 0)
        candidate++;

    /* Search until we have at least n terms */
    while (term_size < n) {
        /* Only numbers congruent to 17 or 19 modulo 30 are candidates */
        uint64_t r = candidate % 30ULL;
        if (r == 17ULL || r == 19ULL) {
            if (is_prime(candidate)) {
                /* Check isolation: neither p‑2 nor p+2 is prime */
                if (!is_prime(candidate - 2ULL) && !is_prime(candidate + 2ULL)) {
                    grow_cache(term_size + 1);
                    term_cache[term_size++] = (seq_t)candidate;
                }
            }
        }
        candidate += 2ULL;  /* next odd number */
    }

    return term_cache[n - 1];
}
