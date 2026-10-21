/*
 * A221443  Number of n X 5 binary arrays with every 0 a horizontal or
 *          antidiagonal neighbor to some 1.
 * https://oeis.org/A221443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) +168*a(n-2) +464*a(n-3) +442*a(n-4) +30*a(n-5)
 *            -88*a(n-6) -9*a(n-8) + a(n-9); Initial values: a(1)=17,
 *            a(2)=429, a(3)=10593, a(4)=260557, a(5)=6415089, a(6)=157933741,
 *            a(7)=3888200065, a(8)=95724317069, a(9)=2356654610641;
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

extern seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;   /* number of terms already cached (>=0) */
    static size_t cap = 0;       /* allocated capacity */

    /* base values (a(1) .. a(9)) */
    static const seq_t init[9] = {
        (seq_t)17,
        (seq_t)429,
        (seq_t)10593,
        (seq_t)260557,
        (seq_t)6415089,
        (seq_t)157933741,
        (seq_t)3888200065,
        (seq_t)95724317069,
        (seq_t)2356654610641
    };

    if (n == 0) return 0; /* undefined for n=0, return 0 as placeholder */

    /* grow cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure: terminate program (unlikely in tests) */
            exit(1);
        }
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 9) {
            val = init[i - 1];
        } else {
            /* indices: a(i-k) is stored at cache[(i-k)-1] */
            val = cache[i - 2] * (seq_t)17
                + cache[i - 3] * (seq_t)168
                + cache[i - 4] * (seq_t)464
                + cache[i - 5] * (seq_t)442
                + cache[i - 6] * (seq_t)30
                - cache[i - 7] * (seq_t)88
                - cache[i - 9] * (seq_t)9
                + cache[i -10];
        }
        cache[i - 1] = val;
    }
    cached_n = n;
    return cache[n - 1];
}
