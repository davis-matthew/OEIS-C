/*
 * A162941  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A162941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=7, a(2)=42, a(3)=252,
 *            a(n)=5*a(n-1)+5*a(n-2)+5*a(n-3)-15*a(n-4) for n>=5, and
 *            a(4)=1491 (obtained by applying the recurrence with a constant
 *            term)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    if (n < cache_len)
        return cache[(size_t)n];

    /* ensure capacity */
    if (cache == NULL) {
        cache_cap = 1;
        while (cache_cap <= (size_t)n) cache_cap <<= 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp != NULL) cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)7;
        } else if (i == 2) {
            cache[i] = (seq_t)42;
        } else if (i == 3) {
            cache[i] = (seq_t)252;
        } else {
            seq_t val = 5 * cache[i-1] + 5 * cache[i-2] + 5 * cache[i-3]
                        - 15 * cache[i-4];
            if (i == 4) val += (seq_t)1; /* constant term from numerator */
            cache[i] = val;
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
