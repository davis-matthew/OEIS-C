/*
 * A082639  Numbers k such that 2*k*(k+2) is a square.
 * https://oeis.org/A082639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=0, a(2)=2; for n≥3 a(n)=6*a(n-1)-a(n-2)+4. This is the
 *            recurrence derived from a(n)=A001541(n)-1 (offset 1). The
 *            function computes the sequence with an expanding static cache,
 *            so calls may arrive in any order
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined behaviour for n<1, but return 0 cheaply. */
    if (n < 1) return (seq_t)0;

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of terms stored in cache */

    /* Initial cache with first two terms */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;     /* fallback on allocation failure */
        cache[0] = (seq_t)0;             /* a(1) */
        cache[1] = (seq_t)2;             /* a(2) */
    }

    /* If the requested term is already cached, return it. */
    if (n <= cache_len) return cache[n-1];

    /* Expand the cache to hold terms up to n */
    idx_t new_len = n;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!new_cache) return (seq_t)0;     /* fallback on realloc failure */
    cache = new_cache;
    cache_len = new_len;

    /* Use the recurrence to compute the missing terms.
       prev1 = a(i-1), prev2 = a(i-2) for the current i.  */
    seq_t prev1 = cache[cache_len - 2];   /* a(i-1) for the first new i */
    seq_t prev2 = cache[cache_len - 3];   /* a(i-2) */

    for (idx_t i = cache_len - 1; i < new_len; ++i) { /* i is 0‑based index of the new term */
        seq_t next = (seq_t)6 * prev1 - prev2 + (seq_t)4;
        cache[i] = next;
        prev2 = prev1;
        prev1 = next;
    }

    return cache[n-1];
}
