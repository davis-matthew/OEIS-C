/*
 * A167183  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0..13) as supplied list; a(14) = 22*∑_{i=1}^{13}a(i) –
 *            253*a(0) + 1; for n>14: a(n) = 22*∑_{i=n-13}^{n-1}a(i) –
 *            253*a(n-14)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* initial 14 terms (a(0) … a(13)) */
static const seq_t init[14] = {
    (seq_t)1,
    (seq_t)24,
    (seq_t)552,
    (seq_t)12696,
    (seq_t)292008,
    (seq_t)6716184,
    (seq_t)154472232,
    (seq_t)3552861336,
    (seq_t)81715810728,
    (seq_t)1879463646744,
    (seq_t)43227663875112,
    (seq_t)994236269127576,
    (seq_t)22867434189934248,
    (seq_t)525950986368487704
};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;       /* memoised values */
    static size_t  cache_cap = 0;     /* allocated length */
    static size_t  cache_len = 0;     /* number of values currently stored */

    size_t idx = (size_t)n;
    if (idx < cache_len)                /* already computed */
        return cache[idx];

    /* grow the cache if needed */
    size_t need = idx + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* in a real project we would check tmp==NULL; omitted for brevity */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new values */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i < 14) {                                    /* initial terms */
            cache[i] = init[i];
        } else {
            seq_t sum13 = 0;
            for (size_t j = i - 13; j < i; ++j)
                sum13 += cache[j];

            if (i == 14) {                               /* special start */
                cache[i] = 22 * sum13 - 253 * cache[0] + 1;
            } else {                                     /* recurrence */
                cache[i] = 22 * sum13 - 253 * cache[i - 14];
            }
        }
    }
    cache_len = need;
    return cache[idx];
}
