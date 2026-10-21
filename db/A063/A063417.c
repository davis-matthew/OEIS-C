/*
 * A063417  Ninth column (k=8) of septinomial array A063265.
 * https://oeis.org/A063417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the ninth column (k=8) of the septinomial array A063265.
 *            a(n) = A063265(n+2,8) = (n+1)*(n+2)*(n^6 +41*n^5 +701*n^4
 *            +6439*n^3 +33930*n^2 +100008*n +100800)/8! a(0)=5, a(1)=36,
 *            a(2)=149, a(3)=470, a(4)=1251, a(5)=2954, a(6)=6371, a(7)=12789,
 *            a(8)=24210. Recurrence for n≥9: a(n) = 9*a(n-1) -...
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (cache == NULL) {
        cache_cap = 9;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 5;
        cache[1] = 36;
        cache[2] = 149;
        cache[3] = 470;
        cache[4] = 1251;
        cache[5] = 2954;
        cache[6] = 6371;
        cache[7] = 12789;
        cache[8] = 24210;
        cache_len = 9;
    }

    if (n < cache_len) return cache[n];

    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 9 * cache[i-1] - 36 * cache[i-2] + 84 * cache[i-3]
                 - 126 * cache[i-4] + 126 * cache[i-5] - 84 * cache[i-6]
                 + 36 * cache[i-7] - 9 * cache[i-8] + cache[i-9];
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
