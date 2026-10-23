/*
 * A037020  Numbers whose sum of proper (or aliquot) divisors is a prime.
 * https://oeis.org/A037020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 3357, u32 n <= 6566, u64 n <= 10000,
 *            u128 n <= 7227, big any n
 * Helpers:   alloc_seq, grow_seq, is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Defines a(n): the nth integer whose sum of proper divisors is a prime.
   Computation is done by incrementally testing successive integers,
   caching the results so that calls in any order are O(1) after the
   first request for each new term. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"
#include "../utils/alloc_seq.h"
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;          /* array of found terms */
static size_t  cache_cap = 0;        /* allocated capacity */
static idx_t   found     = 0;        /* number of terms cached */
static idx_t   checked   = 0;        /* last integer examined */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1, but guard anyway */

    /* Ensure we have at least n terms cached. */
    if (found < n) {
        /* Grow the cache if necessary */
        if (found == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        idx_t i = checked + 1;
        while (found < n) {
            seq_t s = sigma_of(i) - (seq_t)i;   /* sum of proper divisors */
            if (is_prime_big(s)) {
                cache[found++] = (seq_t)i;
            }
            ++i;
        }
        checked = i - 1;
    }

    return cache[n - 1];
}
