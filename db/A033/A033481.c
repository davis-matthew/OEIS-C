/*
 * A033481  3x+1 sequence beginning at 21.
 * https://oeis.org/A033481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=21, a(1)=64, a(n) = a(n-1)/2 if a(n-1) even else a(n) =
 *            3*a(n-1)+1 (3x+1 Collatz sequence starting at 21)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_capacity((size_t)(n + 1));

    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
            cache[0] = (seq_t)21;
        else if (i == 1)
            cache[1] = (seq_t)64;
        else
        {
            seq_t prev = cache[i - 1];
            if (prev & (seq_t)1) /* odd */
                cache[i] = prev * (seq_t)3 + (seq_t)1;
            else /* even */
                cache[i] = prev / (seq_t)2;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
