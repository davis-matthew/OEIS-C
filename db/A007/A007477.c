/*
 * A007477  Shifts 2 places left when convolved with itself.
 * https://oeis.org/A007477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{k=0..n-2} a(k)*a(n-2-k), equivalently the D-finite
 *            recurrence (n+2)*a(n) = -(n+1)*a(n-1) + 4*(n-1)*a(n-2) +
 *            2*(4*n-9)*a(n-3) + 2*(2*n-7)*a(n-4), a(0) = 1, a(1) = 1, a(2) =
 *            1, a(3) = 2
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 31, u64 n <= 57, u128 n <= 110,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_cap = 16;
        cache = alloc_zero(cache_cap);
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 2;
        cache_len = 4;
    }
    if (n < cache_len)
        return cache[n];

    if (n >= cache_cap) {
        idx_t nc = cache_cap;
        while (nc <= n)
            nc *= 2;
        cache = grow_zero(cache, cache_cap, nc);
        cache_cap = nc;
    }

    bool fixed = seq_is_fixed_width();
    for (idx_t i = cache_len; i <= n; i++) {
        if (fixed) {
            /* Convolution: exact modulo 2^w, no division needed */
            seq_t s = 0;
            for (idx_t k = 0; k + 2 <= i; k++)
                s += cache[k] * cache[i - 2 - k];
            cache[i] = s;
        } else {
            /* D-finite recurrence, exact division by (i+2) */
            seq_t s = cache[i - 2] * (4 * (i - 1))
                    + cache[i - 3] * (2 * (4 * i - 9))
                    + cache[i - 4] * (2 * (2 * i - 7));
            s -= cache[i - 1] * (i + 1);
            cache[i] = s / (i + 2);
        }
    }
    cache_len = n + 1;
    return cache[n];
}
