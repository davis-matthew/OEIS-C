/*
 * A056468  a(n) = Sum_{k=1..n} k^6*binomial(n,k).
 * https://oeis.org/A056468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 84*a(n-2) + 280*a(n-3) - 560*a(n-4) +
 *            672*a(n-5) - 448*a(n-6) + 128*a(n-7); Initial values: a(0)=0,
 *            a(1)=1, a(2)=66, a(3)=924, a(4)=7400, a(5)=44040, a(6)=217392
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache stores already computed terms */
    static seq_t *cache = NULL;
    static idx_t   capacity   = 0;   /* allocated size of cache   */
    static idx_t   computed_max = 0; /* highest n that has been computed */

    /* handle the first seven terms directly */
    if (n <= 6) {
        switch (n) {
            case 0: return 0;
            case 1: return 1;
            case 2: return 66;
            case 3: return 924;
            case 4: return 7400;
            case 5: return 44040;
            case 6: return 217392;
        }
    }

    /* initialise the cache if this is the first call */
    if (cache == NULL) {
        capacity = (n + 1);
        if (capacity < 7) capacity = 7;
        cache = (seq_t *)realloc(NULL, capacity * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure is unlikely */

        /* initialise the known first seven values */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 66;
        cache[3] = 924;
        cache[4] = 7400;
        cache[5] = 44040;
        cache[6] = 217392;

        computed_max = 6;
    }
    /* grow the cache if the requested index is out of bounds */
    else if (capacity <= n) {
        capacity = (n + 1);
        if (capacity < 7) capacity = 7;
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        /* computed_max stays unchanged – no new values to initialise */
    }

    /* compute any missing terms up to n using the recurrence */
    if (n > computed_max) {
        for (idx_t k = computed_max + 1; k <= n; ++k) {
            seq_t val =
                14 * cache[k-1] - 84 * cache[k-2] + 280 * cache[k-3]
              - 560 * cache[k-4] + 672 * cache[k-5]
              - 448 * cache[k-6] + 128 * cache[k-7];
            cache[k] = val;
        }
        computed_max = n;
    }

    return cache[n];
}
