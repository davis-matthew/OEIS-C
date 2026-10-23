/*
 * A028164  Expansion of 1/((1-4x)(1-10x)(1-11x)(1-12x)).
 * https://oeis.org/A028164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 37; for n ≥ 2, a(n) = 23*a(n-1) - 132*a(n-2) +
 *            (10^(n+1) - 4^(n+1))/6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow_uint, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow_uint.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 37;

    if (cache == NULL) {
        cache_cap = (size_t)(n + 1);
        cache = seq_alloc(cache_cap);
        cache[0] = 1;
        cache[1] = 37;
        cache_len = 2;
    }

    if (n >= cache_len) {
        size_t needed = (size_t)(n + 1);
        if (needed > cache_cap) {
            seq_t *new_cache = seq_realloc(cache, cache_cap, needed);
            cache = new_cache;
            cache_cap = needed;
        }

        seq_t pow10_cur = pow_uint(10, cache_len);
        seq_t pow4_cur  = pow_uint(4, cache_len);

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t term  = 23 * cache[i-1] - 132 * cache[i-2];
            seq_t pow10_next = pow10_cur * (seq_t)10;
            seq_t pow4_next  = pow4_cur  * (seq_t)4;
            term += (pow10_next - pow4_next) / 6;
            cache[i] = term;
            pow10_cur = pow10_next;
            pow4_cur  = pow4_next;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
