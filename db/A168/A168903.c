/*
 * A168903  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For n ≥ 1 a(n) = 32*(a(n-1)+a(n-2)+…+a(n-20)) – 528*a(n-21) +
 *            N(n) where N(0)=1, N(21)=1, N(1…20)=2, N(n)=0 for n>21. Initial
 *            terms: a(0)=1, a(1)=34, etc
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* stored values of a(k) for k < cache_len  */
static size_t cache_len = 0;     /* number of values currently stored   */
static size_t cache_cap = 0;     /* allocated capacity                   */

seq_t a(idx_t n) {
    /* make sure we have enough space in the cache   */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n                */
    if ((size_t)n >= cache_len) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t sum = 0;
            /* 32*(a(i-1)+…+a(i-20))                  */
            for (int k = 1; k <= 20 && i >= (size_t)k; ++k)
                sum += cache[i - k];
            sum *= (seq_t)32;

            seq_t term = 0;
            if (i >= 21)                  /* –528·a(i‑21)         */
                term = cache[i - 21] * (seq_t)528;

            seq_t N;
            if (i == 0 || i == 21) N = 1;
            else if (i <= 20)        N = 2;
            else                     N = 0;

            cache[i] = sum - term + N;
        }
        cache_len = n + 1;
    }
    return cache[(size_t)n];
}
