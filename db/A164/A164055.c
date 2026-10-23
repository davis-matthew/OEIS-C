/*
 * A164055  Triangular numbers that are one plus a perfect square.
 * https://oeis.org/A164055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A164055: Triangular numbers that are one plus a perfect square
   Recurrence:
     a(1) = 1
     a(2) = 10
     a(3) = 325
     a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3)   (n > 3)
   The function stores previously computed terms in a static cache, so
   any order of calls will be handled efficiently.  All arithmetic is
   performed only on the seq_t type.
*/

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Return 0 for indices below the official offset (1). */
    if (n < 1) return (seq_t)0;

    /* Static cache for computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* highest index currently stored */
    static size_t cache_cap = 0;   /* allocated capacity (>= cache_len+1) */

    /* Allocate and initialise cache if this is the first call. */
    if (!cache) {
        cache_cap = 4;                     /* at least indices 0..3 */
        cache_len  = 3;                     /* we know a(1)..a(3) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                /* allocation failure */
        cache[0] = (seq_t)0;                /* unused slot */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)325;
    }

    /* If the requested term is already cached, return it. */
    if ((idx_t)n <= (idx_t)cache_len)
        return cache[n];

    /* Extend the cache up to n using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Ensure we have enough capacity. */
        if ((size_t)i >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= (size_t)i) new_cap = (size_t)i + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* recurrence: a(i) = 35*(a(i-1)-a(i-2)) + a(i-3) */
        seq_t diff = cache[i-1] - cache[i-2];      /* a(i-1) > a(i-2) for i>3 */
        seq_t term = ((seq_t)35) * diff + cache[i-3];
        cache[i] = term;
    }

    cache_len = n;
    return cache[n];
}
