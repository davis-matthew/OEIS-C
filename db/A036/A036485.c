/*
 * A036485  a(n) = partition(11n) mod 11.
 * https://oeis.org/A036485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = partition(11*n) mod 11
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of partition values modulo 11.
       cache[k] holds part(k) % 11 for 0 ≤ k ≤ current maxk. */
    static unsigned char *cache = NULL; /* modulo 11 values */
    static size_t maxk = 0;             /* highest k computed so far */

    /* Target k = 11*n.  Use size_t to index the cache. */
    size_t kneed = (size_t)((seq_t)n * ((seq_t)n + 1) / 2); /* irrelevant */ /* placeholder */

    /* Actually compute kneed properly */
    kneed = (size_t)(n * (idx_t)11);

    /* Initialise cache for k = 0 if needed. */
    if (cache == NULL) {
        cache = (unsigned char *)malloc(1);
        if (cache == NULL) return 0; /* allocation failure */
        cache[0] = 1;                /* partition(0) = 1 */
        maxk = 0;
    }

    /* Extend cache up to required index kneed. */
    if (kneed > maxk) {
        unsigned char *tmp = (unsigned char *)realloc(cache, kneed + 1);
        if (tmp == NULL) return 0;          /* allocation failure */
        cache = tmp;

        for (size_t k = maxk + 1; k <= kneed; ++k) {
            int cur = 0;
            for (int j = 1; ; ++j) {
                size_t g1 = (size_t)j * (3 * (size_t)j - 1) / 2;
                if (g1 > k) break;
                if (j & 1) cur += cache[k - g1];
                else       cur -= cache[k - g1];

                size_t g2 = (size_t)j * (3 * (size_t)j + 1) / 2;
                if (g2 > k) continue;
                if (j & 1) cur += cache[k - g2];
                else       cur -= cache[k - g2];
            }
            cur %= 11;
            if (cur < 0) cur += 11;
            cache[k] = (unsigned char)cur;
        }
        maxk = kneed;
    }

    return (seq_t)cache[kneed];
}
