/*
 * A212589  Walks with n steps on the x-axis using steps {1,0,-1} and visiting
 *          no point more than twice.
 * https://oeis.org/A212589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   G.f. = (1+x²+x³-3x⁴+2x⁵+x⁶-x⁷)/((1-x)²·(1-x-x²-x⁴)), which gives
 *            the linear recurrence a(0)=1, a(1)=3, a(2)=8, a(3)=18, a(4)=34,
 *            a(5)=64, a(6)=118, a(7)=214 and for n ≥ 8 a(n)=3 a(n-1) – 2
 *            a(n-2) – a(n-3)+2 a(n-4) – 2 a(n-5)+a(n-6)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to store already-computed terms */
    static seq_t *cache = NULL;
    static idx_t size = 0;        /* number of elements stored (size == cache length) */

    /* first ensure cache accommodates n */
    if (n >= size) {
        /* enlarge if necessary */
        idx_t newsize = n + 1;
        if (newsize < 8) newsize = 8;           /* at least store first 8 terms */
        seq_t *newcache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!newcache) {abort();}               /* allocation failure */
        cache = newcache;

        /* initialize first terms on first allocation */
        if (size == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)3;
            cache[2] = (seq_t)8;
            cache[3] = (seq_t)18;
            cache[4] = (seq_t)34;
            cache[5] = (seq_t)64;
            cache[6] = (seq_t)118;
            cache[7] = (seq_t)214;
        }

        /* compute any missing terms */
        for (idx_t i = size; i <= n; ++i) {
            if (i < 8) continue;  /* already set above */
            cache[i] = 3 * cache[i-1]
                     - 2 * cache[i-2]
                     -   cache[i-3]
                     + 2 * cache[i-4]
                     - 2 * cache[i-5]
                     +   cache[i-6];
        }
        size = newsize;
    }
    return cache[n];
}
