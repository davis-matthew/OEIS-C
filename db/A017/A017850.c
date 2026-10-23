/*
 * A017850  Expansion of 1/(1-x^6-x^7-x^8-x^9-x^10).
 * https://oeis.org/A017850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6)+a(n-7)+a(n-8)+a(n-9)+a(n-10) for n>9 Initial
 *            values: a(0)=1, a(1..5)=0, a(6..10)=1
 * Range:     u8 n <= 36, u16 n <= 63, u32 n <= 117, u64 n <= 224,
 *            u128 n <= 439, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* for size_t */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated length of cache */
    static idx_t   last  = 0;   /* greatest accessed index */

    /* Allocate and initialise the cache on the first call */
    if (cache == NULL) {
        cap = 11;                     /* indices 0..10 */
        cache = seq_alloc(cap);
        if (cache == NULL) return 0;  /* allocation failure */

        cache[0] = 1;
        for (idx_t i = 1; i <= 5; ++i) cache[i] = 0;
        for (idx_t i = 6; i <= 10; ++i) cache[i] = 1;
        last = 10;
    }

    /* Return already computed values */
    if (n <= last) return cache[n];

    /* Grow the cache if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        if (tmp == NULL) return 0;   /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute new values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i-6] + cache[i-7] + cache[i-8] + cache[i-9] + cache[i-10];
    }
    last = n;
    return cache[n];
}
