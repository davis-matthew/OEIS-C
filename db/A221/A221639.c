/*
 * A221639  a(n) = 5^(5*n+1) + 4^(5*n+2) + 3^(5*n).
 * https://oeis.org/A221639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(5*n+1) + 4^(5*n+2) + 3^(5*n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (derived from generating function):
   a(0) = 22,       a(1) = 32252,      a(2) = 65664390
   a(n) = 4392*a(n-1) - 4208207*a(n-2) + 777600000*a(n-3)   (n >= 3) */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated capacity */
    static idx_t  last  = 0;     /* highest index already computed */

    /* Base cases */
    if (n == 0) return (seq_t)22;
    if (n == 1) return (seq_t)32252;
    if (n == 2) return (seq_t)65664390;

    /* Initial allocation */
    if (cache == NULL) {
        cap = (n + 1 > 3 ? n + 1 : 3);   /* at least 3 for the bases */
        cache = seq_alloc(cap);
        cache[0] = (seq_t)22;
        cache[1] = (seq_t)32252;
        cache[2] = (seq_t)65664390;
        last = 2;
    }
    /* Grow the cache if needed */
    else if ((size_t)(n + 1) > cap) {
        cache = seq_realloc(cache, cap, n + 1);
        cap   = n + 1;
    }

    /* Return cached value if already computed */
    if (n <= last) return cache[n];

    /* Compute missing terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1] * (seq_t)4392;
        seq_t t2 = cache[i - 2] * (seq_t)4208207;
        seq_t t3 = cache[i - 3] * (seq_t)777600000;
        cache[i] = t1 - t2 + t3;
    }
    last = n;
    return cache[n];
}
