/*
 * A116736  Number of permutations of length n which avoid the patterns 1432,
 *          2314, 2413.
 * https://oeis.org/A116736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 17*a(n-2) + 18*a(n-3) - 7*a(n-4) + a(n-5)
 *            Initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=75
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t base5[5] = {
    (seq_t)1, (seq_t)2, (seq_t)6,
    (seq_t)21, (seq_t)75
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

static void ensure_capacity(idx_t n)
{
    if ((size_t)n > cache_cap)
    {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = (size_t)n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    ensure_capacity(n);
    while (cache_len < n)
    {
        idx_t i = cache_len + 1;
        seq_t val;
        if (i <= 5)
        {
            val = base5[i - 1];
        }
        else
        {
            seq_t a1 = cache[i - 2];
            seq_t a2 = cache[i - 3];
            seq_t a3 = cache[i - 4];
            seq_t a4 = cache[i - 5];
            seq_t a5 = cache[i - 6];
            val = a1 * 7;
            val -= a2 * 17;
            val += a3 * 18;
            val -= a4 * 7;
            val += a5;
        }
        cache[i - 1] = val;
        ++cache_len;
    }
    return cache[n - 1];
}
