/*
 * A037726  Base-7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,3,1.
 * https://oeis.org/A037726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*a(n-1) + a(n-4) - 7*a(n-5) with initial values a(1)=2,
 *            a(2)=14, a(3)=101, a(4)=708, a(5)=4958
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static cache for computed terms */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;   /* how many terms are stored */

static void
ensure(idx_t n)
{
    if (cache_len >= n)
        return;

    /* allocate/grow the cache */
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * n);
    if (cache_len > 0) {
        memcpy(new_cache, cache, sizeof(seq_t) * cache_len);
        free(cache);
    }
    cache = new_cache;

    /* provide the five initial values if they are missing */
    if (cache_len < 5) {
        static const seq_t init[5] = {
            (seq_t)2,
            (seq_t)14,
            (seq_t)101,
            (seq_t)708,
            (seq_t)4958
        };
        idx_t up_to = (n < 5) ? n : 5;
        for (idx_t i = cache_len + 1; i <= up_to; ++i) {
            cache[i-1] = init[i-1];
        }
        cache_len = up_to;
    }

    /* compute any remaining terms up to n using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* a(i) = 7*a(i-1) + a(i-4) - 7*a(i-5) */
        seq_t a_im1 = cache[i-2];   /* a(i-1)   */
        seq_t a_im4 = cache[i-5];   /* a(i-4)   */
        seq_t a_im5 = cache[i-6];   /* a(i-5)   */
        cache[i-1] = 7 * a_im1 + a_im4 - 7 * a_im5;
    }
    cache_len = n;
}

seq_t
a(idx_t n)
{
    if (n <= 5) {
        static const seq_t init[5] = {
            (seq_t)2,
            (seq_t)14,
            (seq_t)101,
            (seq_t)708,
            (seq_t)4958
        };
        return init[n-1];
    }
    ensure(n);
    return cache[n-1];
}
