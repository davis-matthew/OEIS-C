/*
 * A076607  a(0)=1 and for n>0: a(n) = if gcd(a(n-1),n)>1 then lcm(a(n-1),n)
 *          else a(n-1)+n.
 * https://oeis.org/A076607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if gcd(a(n-1),n)>1 then lcm(a(n-1),n) else a(n-1)+n.
 *            Initial value: a(0) = 1
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 45, u128 n <= 81,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void grow_cache_to(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort(); /* OOM – cannot recover */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    grow_cache_to(n + 1);

    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t g = gcd(prev, (seq_t)i);
        seq_t val;
        if (g > (seq_t)1) {
            val = (prev / g) * (seq_t)i;   /* lcm */
        } else {
            val = prev + (seq_t)i;
        }
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
