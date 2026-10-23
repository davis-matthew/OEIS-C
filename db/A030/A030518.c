/*
 * A030518  Number of walks of length n between two vertices on an icosahedron
 *          at distance 2.
 * https://oeis.org/A030518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 4*a(n-1) + 10*a(n-2) - 20*a(n-3) - 25*a(n-4), with
 *            initial values a(1)=0, a(2)=2, a(3)=8, a(4)=52. This linear
 *            recurrence holds for all n ≥ 5
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values (index 0 unused except for internal safety). */
static seq_t *cache = NULL;
static idx_t  cache_cap = 0;  /* capacity of cache array (elements 0..cache_cap-1) */
static idx_t  cache_len = 0;  /* highest computed index stored in cache */

/* Ensure that cache can hold indices 0..n.  The array grows
   geometrically to keep reallocations infrequent. */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;                     /* already enough space */

    idx_t new_cap = n + 1;           /* need at least indices 0..n */
    if (new_cap < 5)                 /* small sequences may need a few slots */
        new_cap = 5;
    if (new_cap < cache_cap * 2)     /* double when needed */
        new_cap = cache_cap * 2;

    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)                  /* allocation failure – caller handles */
        return;

    /* Copy existing data */
    for (idx_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];

    /* Free old storage and update pointers */
    if (cache)
        free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence.  The function memoises
   computed values in a static array so that arbitrary orders of
   calls are efficient. */
seq_t a(idx_t n)
{
    if (n < 1)      /* undefined for n = 0, but return 0 for safety */
        return (seq_t)0;

    ensure_capacity(n);

    /* If the value has already been computed, return it. */
    if (n <= cache_len)
        return cache[n];

    /* Initialise the first four values on first use. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;      /* not used by the recurrence but safe */
        cache[1] = (seq_t)0;      /* a(1) */
        cache[2] = (seq_t)2;      /* a(2) */
        cache[3] = (seq_t)8;      /* a(3) */
        cache[4] = (seq_t)52;     /* a(4) */
        cache_len = 4;
    }

    /* Compute missing terms up to n.  The recurrence only applies for
       indices ≥ 5, therefore the loop starts at max(cache_len+1, 5). */
    idx_t start = cache_len + 1;
    if (start < 5)
        start = 5;

    for (idx_t k = start; k <= n; ++k) {
        seq_t term1 = (seq_t)4 * cache[k - 1];
        seq_t term2 = (seq_t)10 * cache[k - 2];
        seq_t term3 = (seq_t)20 * cache[k - 3];
        seq_t term4 = (seq_t)25 * cache[k - 4];
        cache[k] = term1 + term2 - term3 - term4;
    }

    cache_len = n;
    return cache[n];
}
