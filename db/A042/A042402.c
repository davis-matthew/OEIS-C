/*
 * A042402  Numerators of continued fraction convergents to sqrt(728).
 * https://oeis.org/A042402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A042402: Numerators of continued‑fraction convergents to sqrt(728)
    Recurrence: a(n) = 54*a(n-2) - a(n-4)
    Initial values: a(0)=26, a(1)=27, a(2)=1430, a(3)=1457                */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* capacity in terms of array entries   */
    static idx_t  cache_len  = 0;   /* number of terms computed so far      */

    /* ensure the backing array is large enough for indices [0 .. n] */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap : 4;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;          /* double the capacity */

        /* cast the void* returned by realloc to seq_t* */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure is fatal */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0)
            val = (seq_t)26;
        else if (i == 1)
            val = (seq_t)27;
        else if (i == 2)
            val = (seq_t)1430;
        else if (i == 3)
            val = (seq_t)1457;
        else
            val = 54 * cache[i - 2] - cache[i - 4];

        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
