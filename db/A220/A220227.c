/*
 * A220227  Majority value maps: number of 2 X n binary arrays indicating the
 *          locations of corresponding elements equal to at least half of
 *          their horizontal, diagonal and antidiagonal neighbors in a random
 *          0..1 2 X n array.
 * https://oeis.org/A220227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = 2*a(n-1) + a(n-2) + 6*a(n-3) + a(n-4) + 3*a(n-5)
          - 4*a(n-6) - 2*a(n-7) - 4*a(n-8)
   with initial values a(1)=1, a(2)=6, a(3)=18, a(4)=51,
   a(5)=157, a(6)=477, a(7)=1445, a(8)=4384
   (valid for n ≥ 9) */

#include <stdlib.h>

static inline seq_t mul_small(seq_t x, unsigned int m)
{
    seq_t r = 0;
    for (unsigned int i = 0; i < m; ++i)
        r += x;
    return r;
}

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (cache_cap >= n + 1)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        abort();
    if (cache)
    {
        for (idx_t i = 0; i < cache_cap; ++i)
            new_arr[i] = cache[i];
        free(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

static void init_cache(void)
{
    ensure_cache(8);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)6;
    cache[3] = (seq_t)18;
    cache[4] = (seq_t)51;
    cache[5] = (seq_t)157;
    cache[6] = (seq_t)477;
    cache[7] = (seq_t)1445;
    cache[8] = (seq_t)4384;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if (cache == NULL)
        init_cache();

    if (n < 9)
        return cache[n];

    ensure_cache(n);

    for (idx_t i = 9; i <= n; ++i)
    {
        /* Only compute once, though the loop guarantees that
           each i is visited in increasing order */
        if (cache[i] != (seq_t)0)
            continue;

        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];
        seq_t a7 = cache[i - 7];
        seq_t a8 = cache[i - 8];

        seq_t pos = mul_small(a1, 2);
        pos += a2;
        pos += mul_small(a3, 6);
        pos += a4;
        pos += mul_small(a5, 3);

        seq_t neg = mul_small(a6, 4);
        neg += mul_small(a7, 2);
        neg += mul_small(a8, 4);

        cache[i] = pos - neg;
    }

    return cache[n];
}
