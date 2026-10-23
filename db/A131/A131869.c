/*
 * A131869  Expansion of series reversion of x*(1-8*x)/(1-x).
 * https://oeis.org/A131869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Sequence A131869
   Recurrence: n*a(n) = 15*(2*n-3)*a(n-1) - (n-3)*a(n-2)
   with initial values a(1)=1, a(2)=7.
   Indexing starts at 1 (a(1) is the first term). */

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 in that case. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache for memoisation (indices 0..last). */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* capacity of cache array (>= last+1) */
    static idx_t  last  = 0;   /* largest index computed so far */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cap = 4;  /* enough for indices 0..3 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            exit(1);          /* allocation failure */
        cache[0] = (seq_t)0;  /* unused */
        cache[1] = (seq_t)1;  /* a(1) */
        cache[2] = (seq_t)7;  /* a(2) */
        last   = 2;
    }

    /* Return cached value if already known. */
    if (n <= last)
        return cache[n];

    /* Grow cache as needed. */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            exit(1);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute pending terms. */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
            continue;
        }
        if (i == 2) {
            cache[2] = (seq_t)7;
            continue;
        }

        /* Use 128-bit intermediate for seq_t sizes ≤ 64 bits to avoid overflow. */
        if (sizeof(seq_t) <= 8) {
            __int128 t1 = (__int128)cache[i - 1] * 15;
            t1 *= (__int128)(2 * i - 3);
            __int128 t2 = (__int128)cache[i - 2] * (i - 3);
            __int128 num = t1 - t2;            /* exact numerator */
            cache[i] = (seq_t)(num / i);       /* exact division */
        } else {
            seq_t t1 = cache[i - 1];
            t1 = (t1 * (seq_t)15);
            t1 = (t1 * (seq_t)(2 * i - 3));
            seq_t t2 = cache[i - 2];
            t2 = (t2 * (seq_t)(i - 3));
            cache[i] = (t1 - t2) / (seq_t)i;   /* exact division */
        }
    }

    last = n;
    return cache[n];
}
