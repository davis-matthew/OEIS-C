/*
 * A192344  Constant term of the reduction of n-th polynomial at A161516 by
 *          x^2->x+1.
 * https://oeis.org/A192344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+10*a(n-2)-6*a(n-3)-9*a(n-4) Initial values:
 *            a(1)=1, a(2)=0, a(3)=5, a(4)=4
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0 */

    size_t N = (size_t)n;          /* number of terms needed */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cached_terms = 0;  /* highest index + 1 that is computed */

    /* Allocate or enlarge the cache */
    if (cache == NULL || cache_size < N) {
        seq_t *tmp = (seq_t *)realloc(cache, N * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cache_size = N;
    }

    /* Ensure the first four base values are set if required */
    if (cached_terms < 4) {
        size_t limit = 4;
        if (limit > N) limit = N;
        for (size_t i = cached_terms; i < limit; ++i) {
            switch (i) {
                case 0: cache[i] = (seq_t)1; break; /* a(1) */
                case 1: cache[i] = (seq_t)0; break; /* a(2) */
                case 2: cache[i] = (seq_t)5; break; /* a(3) */
                case 3: cache[i] = (seq_t)4; break; /* a(4) */
            }
        }
        if (limit > cached_terms) cached_terms = limit;
    }

    /* Compute any missing terms up to N-1 (index N-1) */
    for (size_t i = cached_terms; i < N; ++i) {
        /* Recurrence: a(n) = 2*a(n-1)+10*a(n-2)-6*a(n-3)-9*a(n-4) */
        seq_t t1 = cache[i-1] * (seq_t)2;
        seq_t t2 = cache[i-2] * (seq_t)10;
        seq_t t3 = cache[i-3] * (seq_t)6;
        seq_t t4 = cache[i-4] * (seq_t)9;
        cache[i] = (t1 + t2) - (t3 + t4);
    }
    cached_terms = N;

    return cache[N-1];  /* offset 1 → index N-1 */
}
