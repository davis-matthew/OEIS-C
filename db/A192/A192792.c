/*
 * A192792  Molecular topological indices of the Apollonian graphs.
 * https://oeis.org/A192792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 29*a(n-1) – 320*a(n-2) + 1676*a(n-3) – 4109*a(n-4)
          + 2537*a(n-5) + 9182*a(n-6) – 26346*a(n-7) + 43920*a(n-8)
          – 49896*a(n-9) + 23328*a(n-10)
   Initial values:
   a(1)=72, a(2)=360, a(3)=2556, a(4)=22572,
   a(5)=219636, a(6)=2204244, a(7)=22197420,
   a(8)=222257988, a(9)=2207645892, a(10)=21754722852
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* invalid index */
    if (n == 0) return 0;

    /* Base values, needed for n ≤ 10 */
    static const seq_t base[10] = {
        (seq_t)72,          /* a(1)  */
        (seq_t)360,         /* a(2)  */
        (seq_t)2556,        /* a(3)  */
        (seq_t)22572,       /* a(4)  */
        (seq_t)219636,      /* a(5)  */
        (seq_t)2204244,     /* a(6)  */
        (seq_t)22197420,    /* a(7)  */
        (seq_t)222257988,   /* a(8)  */
        (seq_t)2207645892,  /* a(9)  */
        (seq_t)21754722852  /* a(10) */
    };

    /* Memoization cache (0‑based, so cache[0] == a(1)) */
    static seq_t *cache   = NULL;
    static idx_t  cache_size = 0;

    /* Expand cache to at least n elements if needed */
    if (cache_size < n) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)          /* allocation failure */
            return 0;
        cache = tmp;

        /* Copy base values for indices that have not yet been set */
        for (idx_t i = cache_size; i < new_size && i < 10; ++i)
            cache[i] = base[i];

        /* Compute remaining terms using the recurrence
           starting from index 10 (a(11) corresponds to i = 10) */
        for (idx_t i = (cache_size > 10 ? cache_size : 10);
             i < new_size; ++i) {

            seq_t res = 0;
            res += cache[i-1] * (seq_t)29;
            res -= cache[i-2] * (seq_t)320;
            res += cache[i-3] * (seq_t)1676;
            res -= cache[i-4] * (seq_t)4109;
            res += cache[i-5] * (seq_t)2537;
            res += cache[i-6] * (seq_t)9182;
            res -= cache[i-7] * (seq_t)26346;
            res += cache[i-8] * (seq_t)43920;
            res -= cache[i-9] * (seq_t)49896;
            res += cache[i-10] * (seq_t)23328;

            cache[i] = res;
        }

        cache_size = new_size;
    }

    return cache[n-1];
}
