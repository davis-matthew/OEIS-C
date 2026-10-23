/*
 * A029032  Expansion of 1/((1-x)*(1-x^3)*(1-x^4)*(1-x^5)).
 * https://oeis.org/A029032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts {1,3,4,5}.
 *            Recurrence: a(0)=1,
 *            a(n)=a(n-1)+a(n-3)-a(n-6)-a(n-7)+a(n-10)+a(n-12)-a(n-13), with
 *            a(k)=0 for k<0
 * Range:     u8 n <= 38, u16 n <= 280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;

    /* first call: allocate array for at least n+1 entries */
    if (!cache) {
        idx_t size = (n + 1 > 1 ? n + 1 : 1);
#ifdef __cplusplus
        cache = new seq_t[size];
#else
        cache = (seq_t *)malloc(size * sizeof(seq_t));
#endif
        cache[0] = 1;                     /* a(0) */
        for (idx_t i = 1; i <= n; ++i) {   /* compute up to n */
            seq_t val = cache[i - 1];
            if (i >= 3) val += cache[i - 3];
            if (i >= 6) val -= cache[i - 6];
            if (i >= 7) val -= cache[i - 7];
            if (i >= 10) val += cache[i - 10];
            if (i >= 12) val += cache[i - 12];
            if (i >= 13) val -= cache[i - 13];
            cache[i] = val;
        }
        cached_max = n;
        return cache[n];
    }

    /* enlarge cache if necessary */
    if (n > cached_max) {
        idx_t current_size = (cached_max == 0 ? 1 : cached_max);
        idx_t new_size = current_size * 2;
        while (new_size <= n) new_size <<= 1;

#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_size];
#else
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
#endif
        for (idx_t i = 0; i <= cached_max; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;

        /* compute new entries only */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            seq_t val = cache[i - 1];
            if (i >= 3) val += cache[i - 3];
            if (i >= 6) val -= cache[i - 6];
            if (i >= 7) val -= cache[i - 7];
            if (i >= 10) val += cache[i - 10];
            if (i >= 12) val += cache[i - 12];
            if (i >= 13) val -= cache[i - 13];
            cache[i] = val;
        }
        cached_max = n;
    }

    return cache[n];
}
