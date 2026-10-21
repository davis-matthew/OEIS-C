/*
 * A220713  Number of ways to reciprocally link elements of an n X 2 array
 *          either to themselves or to exactly two king-move neighbors,
 *          without consecutive collinear links.
 * https://oeis.org/A220713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-2) + 3*a(n-3) - 2*a(n-4) - 3*a(n-5) +
 *            a(n-6), with a(1)=1, a(2)=8, a(3)=26, a(4)=118, a(5)=463,
 *            a(6)=1930
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;   /* cache[0] unused, cache[i]=a(i) */
    static idx_t cache_n = 0;     /* highest index currently cached */

    if (n == 0) return 0;  /* out of bounds guard */

    /* grow cache if necessary */
    if (n > cache_n) {
        /* allocate or enlarge array to include indices up to n */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) {  /* allocation failed – abort */
            abort();
        }
        cache = new_cache;

        /* compute terms from cache_n+1 up to n */
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            if (i == 1) cache[1] = (seq_t)1;
            else if (i == 2) cache[2] = (seq_t)8;
            else if (i == 3) cache[3] = (seq_t)26;
            else if (i == 4) cache[4] = (seq_t)118;
            else if (i == 5) cache[5] = (seq_t)463;
            else if (i == 6) cache[6] = (seq_t)1930;
            else {
                /* apply recurrence – all operations stay within seq_t */
                cache[i] =  (seq_t)2 * cache[i-1]
                         + (seq_t)8 * cache[i-2]
                         + (seq_t)3 * cache[i-3]
                         - (seq_t)2 * cache[i-4]
                         - (seq_t)3 * cache[i-5]
                         +              cache[i-6];
            }
        }
        cache_n = n;
    }

    return cache[n];
}
