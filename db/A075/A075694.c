/*
 * A075694  a(1)=1, then "jump over next prime": a(n) = 2
 *          nextprime(a(n-1))-a(n-1).
 * https://oeis.org/A075694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 3595, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n) = 2·nextprime(a(n-1)) – a(n-1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static seq_t *cache   = NULL;   /* array of computed terms (1‑based) */
static idx_t   cache_len = 0;   /* number of stored terms  */
static idx_t   cache_cap = 0;   /* current capacity of cache */

/* Return the a‑th term of OEIS A075694.  The result is cached
   so that arbitrary calls in any order are answered correctly.
   The function is safe for C11  and for C++17 when seq_t is an
   arbitrary‑precision integer type. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 0 */

    /* already computed? */
    if (n <= cache_len) return cache[n-1];

    /* grow cache if necessary */
    if (cache_cap < n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first term if cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute missing terms up to a(n) */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t  prev = cache[i-1];
        uint64_t p   = next_prime((uint64_t)prev);   /* next prime > prev */
        cache[i] = (seq_t)((seq_t)2 * (seq_t)p - prev);
    }
    cache_len = n;
    return cache[n-1];
}
