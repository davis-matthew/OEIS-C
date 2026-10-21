/*
 * A079973  Number of permutations satisfying -k <= p(i) - i <= r and p(i) - i
 *          not in I, i=1..n, with k=1, r=4, I={0,3}.
 * https://oeis.org/A079973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-3)+a(n-5) (for n ≥ 5) Initial values: a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=1
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    if (n >= cache_cap) {
        idx_t new_cap = cache_cap > 0 ? cache_cap << 1 : 16;
        while (new_cap <= n)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = 1;
        } else if (i == 1) {
            cache[i] = 0;
        } else if (i == 2 || i == 3 || i == 4) {
            cache[i] = 1;
        } else {
            seq_t v = cache[i - 2];
            v += cache[i - 3];
            v += cache[i - 5];
            cache[i] = v;
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
