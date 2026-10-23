/*
 * A166467  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+...+a(n-11)-a(n-12) for n >= 13 a(12) =
 *            a(11)+a(10)+...+a(1) (sum of previous 11 terms) Initial values:
 *            a(0)=1, a(1)=3, a(2)=6, a(3)=12, a(4)=24, a(5)=48, a(6)=96,
 *            a(7)=192, a(8)=384, a(9)=768, a(10)=1536, a(11)=3072. This
 *            matches the rational generating function given in OEI...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0; /* number of terms stored */
static size_t cache_cap = 0; /* allocated capacity */

static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (cache_len == 0) {
        ensure_capacity(12);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)24;
        cache[5] = (seq_t)48;
        cache[6] = (seq_t)96;
        cache[7] = (seq_t)192;
        cache[8] = (seq_t)384;
        cache[9] = (seq_t)768;
        cache[10] = (seq_t)1536;
        cache[11] = (seq_t)3072;
        cache_len = 12;
    }

    if (n < cache_len)
        return cache[n];

    ensure_capacity((size_t)(n + 1));
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        for (int j = 1; j <= 11; ++j)
            sum += cache[i - (idx_t)j];
        if (i >= 13)
            sum -= cache[i - 12];
        cache[i] = sum;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
