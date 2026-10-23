/*
 * A142588  A trisection of A000129, the Pell numbers.
 * https://oeis.org/A142588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) + a(n-2), a(0) = 0, a(1) = 5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_capacity = 0;
static idx_t max_computed = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_capacity)
        return;
    idx_t new_cap = cache_capacity ? cache_capacity << 1 : 1;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)5;

    ensure_cache(n);

    if (n > max_computed) {
        if (max_computed == 0) {
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)5;
            max_computed = 1;
        }
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)14 * cache[i - 1] + cache[i - 2];
        }
        max_computed = n;
    }

    return cache[n];
}
