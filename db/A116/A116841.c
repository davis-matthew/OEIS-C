/*
 * A116841  Number of permutations of length n which avoid the patterns 1234,
 *          1324, 2431.
 * https://oeis.org/A116841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 73*a(n-2) + 231*a(n-3) - 448*a(n-4) * +
 *            534*a(n-5) - 349*a(n-6) + 43*a(n-7) + 119*a(n-8) * - 97*a(n-9) +
 *            32*a(n-10) - 4*a(n-11) (n >= 12), * with a(1)=1, a(2)=2, a(3)=6,
 *            a(4)=21, a(5)=75, a(6)=258, * a(7)=842, a(8)=2614, a(9)=7787,
 *            a(10)=22466, a(11)=63273
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[11] = {
    (seq_t)1,(seq_t)2,(seq_t)6,(seq_t)21,(seq_t)75,(seq_t)258,
    (seq_t)842,(seq_t)2614,(seq_t)7787,(seq_t)22466,(seq_t)63273
};

static const int coeff[11] = {
    13, -73, 231, -448, 534, -349, 43, 119, -97, 32, -4
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_max = 0;

static void ensure_capacity(idx_t n)
{
    if (cache && (size_t)n < cache_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    if (new_cap < 64)
        new_cap = 64;
    if (new_cap < cache_cap)
        new_cap = cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) {
        /* allocation failed – keep old cache */
        tmp = cache;
        new_cap = cache_cap;
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n <= 11)
        return init_vals[n - 1];

    ensure_capacity(n);

    if (cached_max < 11) {
        for (int i = 0; i < 11; ++i)
            cache[i] = init_vals[i];
        cached_max = 11;
    }

    for (idx_t k = cached_max + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (int i = 0; i < 11; ++i) {
            const seq_t val = cache[k - 2 - i];   /* a(k-i-1) */
            int c = coeff[i];
            if (c > 0) {
                sum += (seq_t)((seq_t)val * (unsigned int)c);
            } else {
                sum -= (seq_t)((seq_t)val * (unsigned int)(-c));
            }
        }
        cache[k - 1] = sum;
    }
    cached_max = n;
    return cache[n - 1];
}
