/*
 * A141468  Zero together with the nonprime numbers A018252.
 * https://oeis.org/A141468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0; a(n)=A018252(n-1) for n>1
 * Range:     u8 n <= 202, u16 n <= 9774, u32 n <= 9774, u64 n <= 9774,
 *            u128 n <= 9774, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A141468: zero together with nonprime numbers */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of elements stored (index 1..size) */
static size_t cache_cap  = 0;   /* allocated capacity */
static idx_t next_candidate = 1; /* next integer to test for nonprime */

static void ensure_cache_capacity(void)
{
    if (cache_size + 1 <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (cache_size + 1 > new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { abort(); }  /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For other n we simply return 0. */
    if (n == 0) return (seq_t)0;

    /* Initialise cache on first call. */
    if (cache == NULL) {
        ensure_cache_capacity();
        cache[0] = (seq_t)0;   /* a(1) = 0 */
        cache_size = 1;
        next_candidate = 1;    /* start checking from 1 for a(2) */
    }

    if (n <= (idx_t)cache_size) {
        return cache[n - 1];
    }

    /* Grow cache until it contains a(n). */
    while (cache_size < (size_t)n) {
        if (!is_prime(next_candidate)) {
            ensure_cache_capacity();
            cache[cache_size] = (seq_t)next_candidate;
            cache_size++;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
