/*
 * A116640  a(n) = A116623(A059893(n)).
 * https://oeis.org/A116640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) defined by the recurrence a(0) = 1 for n > 0: let h =
 *            ⌊n/2⌋, q = ⌊n/4⌋, r = n mod 4 if r == 0: a(n) = 3·a(h) – 2·a(q)
 *            if r == 1: a(n) = 6·a(q) – a(h) if r == 2: a(n) = a(h) + 2·a(q)
 *            if r == 3: a(n) = 5·a(h) – 6·a(q) Implemented with memoisation
 *            so repeated calls are O(1) after the first eval...
 * Range:     u8 n <= 28, u16 n <= 1014, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static cache for memoised values */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* enlarge cache so that it can hold the entry for index n */
static void ensure_cache_capacity(idx_t n)
{
    if (n < cache_size) return;
    /* choose new size: double until large enough, but at least n+1 */
    idx_t new_size = cache_size;
    if (new_size == 0) new_size = 1;
    while (new_size <= n) new_size <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();              /* fatal: out of memory */
    /* initialise the new part to zero */
    memset(new_cache + cache_size, 0,
           (new_size - cache_size) * sizeof(seq_t));
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* base term */
    if (n == 0) return (seq_t)1;

    /* if already computed, return it */
    if (n < cache_size && cache[n] != (seq_t)0)
        return cache[n];

    /* ensure we can address cache[n] */
    ensure_cache_capacity(n);

    /* compute recursively on smaller indices */
    idx_t h = n >> 1;   /* floor(n/2) */
    idx_t q = n >> 2;   /* floor(n/4) */
    seq_t a_h = a(h);
    seq_t a_q = a(q);

    seq_t result;
    switch (n & 3) { /* n mod 4 */
        case 0:  result = 3 * a_h - 2 * a_q; break;
        case 1:  result = 6 * a_q - a_h;      break;
        case 2:  result = a_h + 2 * a_q;      break;
        case 3:  result = 5 * a_h - 6 * a_q;  break;
    }

    cache[n] = result; /* memoise */
    return result;
}
