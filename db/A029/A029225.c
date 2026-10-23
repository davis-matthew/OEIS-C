/*
 * A029225  Expansion of 1/((1-x^2)*(1-x^6)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-6)-a(n-8)+a(n-11)+a(n-12)
 *            -a(n-13)-a(n-14)-a(n-17)-a(n-18)
 *            +a(n-19)+a(n-20)-a(n-23)+a(n-25)+a(n-29)-a(n-31); Initial
 *            values: a(0)=1, a(1)=0, a(2)=1 ... (OEIS A029225 – partitions of
 *            n into parts 2,6,11,12)
 * Range:     u8 n <= 113, u16 n <= 833, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: allocated sequence values and number of valid entries */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of valid entries in cache */
    static idx_t cache_cap = 0;   /* allocated capacity of cache */

    /* Ensure the cache can hold up to index n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* out‑of‑memory: signal error */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing entries up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        /* Positive contributions */
        static const int pos_offsets[] = {2, 6, 11, 12, 19, 20, 25, 29};
        static const int neg_offsets[] = {8, 13, 14, 17, 18, 23, 31};
        const size_t pos_cnt = sizeof(pos_offsets) / sizeof(pos_offsets[0]);
        const size_t neg_cnt = sizeof(neg_offsets) / sizeof(neg_offsets[0]);

        seq_t pos_sum = 0;
        for (size_t j = 0; j < pos_cnt; ++j) {
            idx_t k = i - (idx_t)pos_offsets[j];
            if (k < i)          /* k >= 0 implied; underflow yields huge > i */
                pos_sum += cache[k];
        }

        seq_t neg_sum = 0;
        for (size_t j = 0; j < neg_cnt; ++j) {
            idx_t k = i - (idx_t)neg_offsets[j];
            if (k < i)
                neg_sum += cache[k];
        }

        cache[i] = pos_sum - neg_sum;
    }

    cache_len = n + 1;
    return cache[n];
}
