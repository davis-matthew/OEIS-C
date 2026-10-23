/*
 * A116157  a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) + a(n-5).
 * https://oeis.org/A116157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) + a(n-5), with initial
 *            values a(0)=1, a(1)=1, a(2)=3, a(3)=3, a(4)=7
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Implementation uses a relaxed memoization scheme; recursion
   is only handled via a simple array cache.  The cache holds
   already computed terms and is grown as needed.  All arithmetic
   is performed using the opaque seq_t type, so the code is
   agnostic to whether seq_t is a built‐in integer type or an
   arbitrary‑precision integer class. */

static seq_t *cache          = NULL;   /* cached sequence values       */
static idx_t  cache_cap      = 0;      /* number of allocated slots    */
static idx_t  cache_len      = 0;      /* next free index (size cached)*/

static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    /* realloc may return NULL on failure; abort to keep behaviour
       deterministic in the contest environment. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of A116157. */
seq_t a(idx_t n)
{
    /* If we already have the term cached, just return it. */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache to hold terms up to n. */
    idx_t needed = n + 1;
    ensure_capacity(needed);

    /* Compute missing terms one by one.  For indices < 5 we use
       the explicit starting values; for i >= 5 we apply the
       recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)3;
        else if (i == 3)
            cache[i] = (seq_t)3;
        else if (i == 4)
            cache[i] = (seq_t)7;
        else {
            seq_t val = cache[i-1] + 2*cache[i-2] - 2*cache[i-3] + cache[i-5];
            cache[i] = val;
        }
    }

    cache_len = needed;
    return cache[n];
}
