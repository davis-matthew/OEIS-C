/*
 * A075825  a(0) = 1, a(1) = 2; for n>0, a(2n) = |a(n)-a(n-1)|, a(2n+1) =
 *          a(n)+a(n-1).
 * https://oeis.org/A075825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 1, a(1) = 2; for n>0, a(2n) = |a(n)-a(n-1)|, a(2n+1) =
 *            a(n)+a(n-1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    /* Ensure the array can hold indices 0 … n. */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        if (new_cap > cache_cap) {
            for (idx_t i = cache_cap; i < new_cap; ++i)
                tmp[i] = (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any values that have not yet been cached. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)2;
        } else if ((i & 1) == 0) {          /* even: a(2k) = |a(k)-a(k-1)| */
            seq_t a = cache[i >> 1];
            seq_t b = cache[(i >> 1) - 1];
            cache[i] = (a > b) ? (a - b) : (b - a);
        } else {                           /* odd: a(2k+1) = a(k)+a(k-1)  */
            cache[i] = cache[i >> 1] + cache[(i >> 1) - 1];
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
