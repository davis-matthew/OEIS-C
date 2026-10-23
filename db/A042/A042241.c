/*
 * A042241  Denominators of continued fraction convergents to sqrt(646).
 * https://oeis.org/A042241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 610*a(n-4) - a(n-8) for n > 7, with a(0)=1, a(1)=2,
 *            a(2)=5, a(3)=12, a(4)=605, a(5)=1222, a(6)=3049, a(7)=7320
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static const seq_t init[8] = {1, 2, 5, 12, 605, 1222, 3049, 7320};

static void ensure_cache(idx_t n)
{
    /* Expand cache if necessary.  The cache always contains the
       first max(n+1,8) values of the sequence.  Initial terms are
       stored from the static array `init`.  Subsequent terms are
       computed using the recurrence. */
    if (cache_len == 0) {
        idx_t need = n + 1;
        if (need < 8) need = 8;
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        for (idx_t i = 8; i < need; ++i)
            cache[i] = (seq_t)610 * cache[i - 4] - cache[i - 8];
        cache_len = need;
    } else if ((n + 1) > cache_len) {
        idx_t newlen = n + 1;
        if (newlen < 8) newlen = 8;
        cache = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
        for (idx_t i = cache_len; i < 8; ++i)
            cache[i] = init[i];
        for (idx_t i = cache_len; i <= n; ++i)
            cache[i] = (seq_t)610 * cache[i - 4] - cache[i - 8];
        cache_len = newlen;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
