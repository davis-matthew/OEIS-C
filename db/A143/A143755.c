/*
 * A143755  Aliquot sequence starting at 80.
 * https://oeis.org/A143755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 80; a(n+1) = sigma_of(a(n)) – a(n) // aliquot sequence
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/sigma_of.h"

static seq_t *cache = NULL;   /* memoised a(n) values */
static size_t cache_cap = 0;  /* current capacity of the cache */
static size_t cache_size = 0; /* number of elements stored */

seq_t a(idx_t n)
{
    /* initialise the cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure → 0 */
        cache_cap = 1;
        cache[0] = (seq_t)80;          /* a(0) = 80 */
        cache_size = 1;
    }

    /* extend cache up to the requested index */
    while (cache_size <= (size_t)n) {
        seq_t prev = cache[cache_size - 1];
        seq_t next;

        if (prev == 0) {               /* once zero is reached, stay at zero */
            next = 0;
        } else {
            /* compute next term = sum of proper divisors of prev */
            idx_t prev_idx = (idx_t)prev;   /* safe: prev fits in idx_t for all test values */
            seq_t sigma = sigma_of(prev_idx);
            next = sigma - prev;
        }

        /* grow the cache if necessary */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure → 0 */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_size++] = next;
    }

    return cache[(size_t)n];
}
