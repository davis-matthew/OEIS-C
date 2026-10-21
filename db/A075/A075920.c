/*
 * A075920  Seventh column of triangle A075501.
 * https://oeis.org/A075920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 168*a(n-1) - 11592*a(n-2) + 423360*a(n-3) -
 *            8772624*a(n-4) + 102114432*a(n-5) - 609700608*a(n-6) +
 *            1410877440*a(n-7) with initial values a(0)=1, a(1)=168,
 *            a(2)=16632, a(3)=1270080, a(4)=82927152, a(5)=4878631296,
 *            a(6)=266658822144
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently stored */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0; /* failure */
        /* store first 7 terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)168;
        cache[2] = (seq_t)16632;
        cache[3] = (seq_t)1270080;
        cache[4] = (seq_t)82927152;
        cache[5] = (seq_t)4878631296ULL;
        cache[6] = (seq_t)266658822144ULL;
        cache_len = 7;
    }

    /* grow cache to contain n */
    while (cache_len <= n) {
        if (cache_len >= cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
            if (!cache) return (seq_t)0; /* failure */
        }
        idx_t i = cache_len;

        /* compute using recurrence */
        seq_t term1 = (seq_t)168 * cache[i-1];
        seq_t term3 = (seq_t)423360 * cache[i-3];
        seq_t term5 = (seq_t)102114432 * cache[i-5];
        seq_t term7 = (seq_t)1410877440 * cache[i-7];

        seq_t term2 = (seq_t)11592 * cache[i-2];
        seq_t term4 = (seq_t)8772624 * cache[i-4];
        seq_t term6 = (seq_t)609700608 * cache[i-6];

        seq_t pos = term1 + term3 + term5 + term7;
        seq_t neg = term2 + term4 + term6;
        cache[i] = pos - neg;

        cache_len++;
    }

    return cache[n];
}
