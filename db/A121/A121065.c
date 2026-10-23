/*
 * A121065  a(n) is the smallest number in English which contains n letter
 *          'E's.
 * https://oeis.org/A121065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = a(n-1) + 1000*a(n-7) - 1000*a(n-8) for n > 9 * Initial
 *            values: a(0)=2, a(1)=0, a(2)=3, a(3)=11, a(4)=17, a(5)=111, *
 *            a(6)=117, a(7)=317, a(8)=1317, a(9)=3317
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const idx_t init_vals[10] = {2, 0, 3, 11, 17, 111, 117, 317, 1317, 3317};
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;   /* allocation failure handled by caller/tests */
        for (size_t i = 0; i < 10; ++i)
            cache[i] = (seq_t)init_vals[i];
        cache_len = 10;
    }
    while (cache_len <= (size_t)n) {
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp) cache = tmp;
        }
        if (cache_len >= 10) {
            const seq_t prev = cache[cache_len - 1];
            const seq_t a7   = cache[cache_len - 7];
            const seq_t a8   = cache[cache_len - 8];
            cache[cache_len] = prev + (seq_t)1000 * a7 - (seq_t)1000 * a8;
        }
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
