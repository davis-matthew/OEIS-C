/*
 * A185883  1/729 the number of (n+2) X 3 0..2 arrays with no 3 X 3 subblock
 *          trace equal to any horizontal or vertical neighbor 3 X 3 subblock
 *          trace.
 * https://oeis.org/A185883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=27, a(2)=588, a(3)=12900, a(4)=282672 a(n) = 10*a(n-1) +
 *            221*a(n-2) + 846*a(n-3) + 756*a(n-4) for n >= 5 This implements
 *            the empirical recurrence of OEIS A185883
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 0 is unused – the sequence is 1‑based */
    static seq_t *cache = NULL;      /* cache[1] … cache[cache_size] */
    static idx_t   cache_size = 0;   /* number of terms cached so far */

    if (n <= 0) return (seq_t)0;     /* undefined for n <= 0, but harmless */

    /* initialise the first four terms on first use */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        cache[1] = (seq_t)27;              /* a(1)  */
        cache[2] = (seq_t)588;             /* a(2)  */
        cache[3] = (seq_t)12900;           /* a(3)  */
        cache[4] = (seq_t)282672;          /* a(4)  */
    }

    /* extend the cache if necessary */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) {           /* allocation failure – abort */
            abort();
        }
        cache = new_cache;
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            cache[k] =
                (seq_t)10  * cache[k - 1] +
                (seq_t)221 * cache[k - 2] +
                (seq_t)846 * cache[k - 3] +
                (seq_t)756 * cache[k - 4];
        }
        cache_size = n;
    }

    return cache[n];
}
