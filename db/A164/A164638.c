/*
 * A164638  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            23*a(n-1)+23*a(n-2)+23*a(n-3)+23*a(n-4)+23*a(n-5)+23*a(n-6)-276*a(n-7)
 *            for n ≥ 8, with initial values a(0)=1, a(1)=25, a(2)=600,
 *            a(3)=14400, a(4)=345600, a(5)=8294400, a(6)=199065600,
 *            a(7)=4777574100
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_terms[8] = {
    (seq_t)1,
    (seq_t)25,
    (seq_t)600,
    (seq_t)14400,
    (seq_t)345600,
    (seq_t)8294400,
    (seq_t)199065600,
    (seq_t)4777574100
};

seq_t a(idx_t n)
{
    if (n < 8)
        return init_terms[n];

    /* dynamic cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with first 8 terms */
    if (cache_len == 0) {
        cache_len = 8;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init_terms[i];
    }

    /* resize if needed */
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) {
            /* memory failure – return zero (caller may handle) */
            return (seq_t)0;
        }
        cache = tmp;
        /* compute newly needed terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = cache[i - 1] * (seq_t)23;
            sum += cache[i - 2] * (seq_t)23;
            sum += cache[i - 3] * (seq_t)23;
            sum += cache[i - 4] * (seq_t)23;
            sum += cache[i - 5] * (seq_t)23;
            sum += cache[i - 6] * (seq_t)23;
            sum -= cache[i - 7] * (seq_t)276;
            cache[i] = sum;
        }
        cache_len = new_len;
    }

    return cache[n];
}
