/*
 * A220564  Number of ways to reciprocally link elements of an 3 X n array
 *          either to themselves or to exactly one horizontal or antidiagonal
 *          neighbor.
 * https://oeis.org/A220564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 15*a(n-2) - 5*a(n-3) - 19*a(n-4) + 9*a(n-5) +
 *            2*a(n-6) - a(n-7), applicable for n ≥ 8. Initial values: a(1)=1,
 *            a(2)=13, a(3)=64, a(4)=430, a(5)=2604, a(6)=16310, a(7)=101052
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[7] = {
    (seq_t)1, (seq_t)13, (seq_t)64, (seq_t)430,
    (seq_t)2604, (seq_t)16310, (seq_t)101052
};

static seq_t *cache      = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

static void init_cache(void)
{
    if (cache_len >= 7) return;
    ensure_capacity(7);
    for (size_t i = 0; i < 7; ++i) cache[i] = init_vals[i];
    cache_len = 7;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n <= 7) return init_vals[n - 1];

    init_cache();                       /* ensure first 7 values are stored */

    if (cache_len < (size_t)n)
    {
        ensure_capacity(n);
        for (idx_t k = (idx_t)cache_len + 1; k <= n; ++k)
        {
            size_t i1 = (size_t)(k - 2);
            size_t i2 = (size_t)(k - 3);
            size_t i3 = (size_t)(k - 4);
            size_t i4 = (size_t)(k - 5);
            size_t i5 = (size_t)(k - 6);
            size_t i6 = (size_t)(k - 7);
            size_t i7 = (size_t)(k - 8);
            seq_t val = (seq_t)4 * cache[i1]
                      + (seq_t)15 * cache[i2]
                      - (seq_t)5 * cache[i3]
                      - (seq_t)19 * cache[i4]
                      + (seq_t)9 * cache[i5]
                      + (seq_t)2 * cache[i6]
                      - cache[i7];
            cache[(size_t)(k - 1)] = val;
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)(n - 1)];
}
