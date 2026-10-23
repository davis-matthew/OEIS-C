/*
 * A036780  Primes of the form n + Sum_{i=1..n} i!.
 * https://oeis.org/A036780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of form n + Σ_{i=1}^{n} i!; a(n) is the n‑th such prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cached primes */
static size_t cache_cap = 0;         /* capacity of cache array */
static size_t cache_used = 0;        /* number of primes stored */

static idx_t   current_n = 0;        /* last n processed */
static seq_t   current_fact = 1;     /* (current_n)!    */
static seq_t   current_sum  = 0;     /* Σ_{i=1}^{current_n} i! */

static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                     /* memory exhausted */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; undefined for 0 */

    /* generate primes until we have at least n terms */
    while (cache_used < n)
    {
        /* move to the next n */
        current_n++;
        current_fact *= current_n;        /* factorial incrementally */
        current_sum  += current_fact;     /* sum of factorials      */

        seq_t val = current_sum + current_n;   /* candidate prime */

        if (is_prime_big(val))           /* prime test */
        {
            if (cache_used == cache_cap) grow_cache();
            cache[cache_used++] = val;
        }
    }

    return cache[n - 1]; /* zero‑based index */
}
