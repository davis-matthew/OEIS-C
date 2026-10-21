/*
 * A234770  Number of (n+1) X (1+1) 0..4 arrays with 2 X 2 edge jumps all no
 *          more than +1 in one of the clockwise or counterclockwise
 *          directions but not both.
 * https://oeis.org/A234770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 21*a(n-2) - a(n-3) - 67*a(n-4) - 10*a(n-5) +
 *            46*a(n-6) Initial values: a(1) = 88 a(2) = 454 a(3) = 2056 a(4)
 *            = 10076 a(5) = 46804 a(6) = 226072
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_sz = 0;

static void ensure_cache_cap(idx_t n)
{
    /* need slots 0..n inclusive */
    size_t needed = (size_t)n + 1;
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

static void init_cache(void)
{
    ensure_cache_cap(6);
    cache[0] = 0;   /* dummy for 0 index */
    cache[1] = 88;
    cache[2] = 454;
    cache[3] = 2056;
    cache[4] = 10076;
    cache[5] = 46804;
    cache[6] = 226072;
    cache_sz = 6;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset starts at 1 */

    if (cache_sz == 0)
        init_cache();

    if (n <= cache_sz)
        return cache[n];

    ensure_cache_cap(n);

    for (idx_t i = cache_sz + 1; i <= n; ++i) {
        seq_t sum_pos = cache[i - 1];
        sum_pos = sum_pos + cache[i - 2] * 21;
        sum_pos = sum_pos + cache[i - 6] * 46;

        seq_t sum_neg = cache[i - 3];
        sum_neg = sum_neg + cache[i - 4] * 67;
        sum_neg = sum_neg + cache[i - 5] * 10;

        cache[i] = sum_pos - sum_neg;
    }

    cache_sz = n;
    return cache[n];
}
