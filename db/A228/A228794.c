/*
 * A228794  Number of n X 6 binary arrays with top left element equal to 1 and
 *          no two ones adjacent horizontally or nw-se.
 * https://oeis.org/A228794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) - 112*a(n-2) + 217*a(n-3) - 157*a(n-4) +
 *            36*a(n-5) for n > 7. Initial values: a(1)=8, a(2)=98, a(3)=1394,
 *            a(4)=19474, a(5)=272952, a(6)=3835792, a(7)=53992646. The
 *            function memoises computed terms to allow arbitrary call order
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* static cache: index 1..cache_size contain a(1)..a(cache_size)   */
    static seq_t *cache = NULL;      /* 1‑based array, element 0 unused   */
    static idx_t cache_size = 0;     /* number of terms stored (>=0)      */

    /* Ensure base capacity for at least 7 terms and set the initial values.  */
    if (cache_size < 7) {
        /* allocate enough room for indices 0..7, i.e. size 8              */
        idx_t new_cap = 8;
        if (cache == NULL) {
            cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        cache_size = 7;
        /* initialise the first seven terms (overwrites only if needed)   */
        cache[1] = 8;
        cache[2] = 98;
        cache[3] = 1394;
        cache[4] = 19474;
        cache[5] = 272952;
        cache[6] = 3835792;
        cache[7] = 53992646;
    }

    /* Grow cache if needed.  The cache array is 1‑based; element 0 is unused. */
    if (cache_size < n) {
        idx_t new_cap = n + 1;                /* include element n         */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* Compute missing terms using the recurrence            */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /*
             * To avoid temporary unsigned underflow we compute
             *   pos = 21*a(i-1) + 217*a(i-3) + 36*a(i-5)
             *   neg = 112*a(i-2) + 157*a(i-4)
             * and then pos - neg.
             */
            seq_t pos = (seq_t)21 * cache[i - 1] +
                        (seq_t)217 * cache[i - 3] +
                        (seq_t)36  * cache[i - 5];
            seq_t neg = (seq_t)112 * cache[i - 2] +
                        (seq_t)157 * cache[i - 4];
            cache[i] = pos - neg;
        }
        cache_size = n;
    }

    return cache[n];
}
