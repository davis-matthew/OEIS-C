/*
 * A201115  Primes of the form 3n^3+5.
 * https://oeis.org/A201115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 3·k³+5, with k starting at 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 111, u64 n <= 2600,
 *            u128 n <= 2600, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 5, a(2) = 29, a(3) = 197, … (OEIS A201115) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

/* Static cache for previously computed values. */
static seq_t *cache = NULL;      /* array of primes of the form 3k³+5 */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t cache_len = 0;      /* number of elements stored */
static idx_t cur_k = 0;          /* next k to test (k = 0,1,2,…) */

/* Ensure the cache has room for at least one more element. */
static void ensure_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    /* Generate primes until we have at least n terms. */
    while (cache_len < n) {
        seq_t k = (seq_t)cur_k;
        seq_t cube = k * k * k;               /* k³ */
        seq_t val = (seq_t)3 * cube + (seq_t)5; /* 3k³+5 */

        if (isPrimeGeneric(val))
            ensure_capacity(), cache[cache_len++] = val;

        cur_k++;          /* advance to next k */
    }
    return cache[n - 1];
}
