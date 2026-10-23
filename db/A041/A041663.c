/*
 * A041663  Denominators of continued fraction convergents to sqrt(350).
 * https://oeis.org/A041663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 898*a(n-6) - a(n-12) with initial values: a(0)=1, a(1)=1,
 *            a(2)=3, a(3)=7, a(4)=17, a(5)=24, a(6)=881, a(7)=905, a(8)=2691,
 *            a(9)=6287, a(10)=15265, a(11)=21552
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 41, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;     /* array of computed terms */
    static size_t cache_len = 0;    /* number of elements in cache  */

    /* initial 12 terms known explicitly */
    static const seq_t init[12] = {
        (seq_t)1,(seq_t)1,(seq_t)3,(seq_t)7,(seq_t)17,(seq_t)24,
        (seq_t)881,(seq_t)905,(seq_t)2691,(seq_t)6287,(seq_t)15265,(seq_t)21552
    };

    /* first call: allocate and populate initial part */
    if (!cache) {
        cache_len = 12;                         /* indices 0..11 stored    */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = init[i];
    }

    /* trivial case: already in cache */
    if (n < cache_len)
        return cache[(size_t)n];

    /* enlarge cache to hold indices up to n */
    size_t new_len = (size_t)(n + 1);
    if (new_len > cache_len) {
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        /* compute missing terms using the recurrence */
        for (size_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)(898) * cache[i - 6] - cache[i - 12];
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
